<?php
// $Id: FeedsXPathParser.inc,v 1.1.2.33 2010/11/04 18:05:55 twistor Exp $

/**
 * @file
 *
 * Provides the HTML and XML Classes for Feeds XPath Parser.
 */

/**
 * Base class for the HTML and XML parsers.
 */
class FeedsXPathParserBase extends FeedsParser {

  protected $modified_queries = array();
  protected $rawXML = array();
  protected $debug = array();

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    $this->source_config = $source->getConfigFor($this);
    $mappings = feeds_importer($this->id)->processor->config['mappings'];
    $this->mappings = array();
    foreach ($mappings as $mapping) {
      if (strpos($mapping['source'], 'xpathparser:') === 0) {
        $this->mappings[$mapping['source']] = $mapping['target'];
      }
    }

    if (isset($this->source_config['rawXML']) && is_array($this->source_config['rawXML'])) {
      $this->rawXML = array_keys(array_filter($this->source_config['rawXML']));
    }
    if (isset($this->source_config['exp']['debug']) && is_array($this->source_config['exp']['debug'])) {
      $this->debug = array_keys(array_filter($this->source_config['exp']['debug']));
    }
  }

  /**
   * Starts the parsing. First runs the context query, then queries each
   * item from the context with the field queries.
   *
   * @param $xml
   *   A SimpleXMLElement object.
   *
   * @return array
   *   Returns a structured array suitable for adding to a batch object with
   *   $batch->setItems().
   */
  protected function query($xml) {
    // Fetch the namespaces defined in the XML document.
    $this->namespaces = $xml->getNamespaces(TRUE);

    $all_items = $this->namespacedXPathQuery($xml, $this->source_config['context']);
    $this->debug($all_items, 'context');
    unset($xml);
    $parsed_items = array();
    foreach ($all_items as $item) {
      $parsed_item = $variables = array();
      foreach ($this->source_config['sources'] as $source => $query) {
        $query = strtr($query, $variables);
        $result = $this->parseSourceElement($item, $query, $source);
        if (!is_array($result)) {
          $variables['$' . $this->mappings[$source]] = $result;
        }
        $parsed_item[$source] = $result;
      }
      $parsed_items[] = $parsed_item;
    }
    return $parsed_items;
  }

  /**
   * Executes an XPath query with namespace support.
   *
   * @param $xml
   *   The XML element to execute the query on.
   *
   * @param $query
   *   An XPath query.
   *
   * @return array
   *   An array containing the results of the query.
   */
  private function namespacedXPathQuery($xml, $query) {

    foreach ($this->namespaces as $prefix => $namespace) {
      if ($prefix === '') {
        $xml->registerXPathNamespace('__default__', $namespace);
        // Replace all the elements without prefix by the default prefix.
        if (!isset($this->modified_queries[$query])) {
          $mod_query = $this->addDefaultNamespace($query);
          $this->modified_queries[$query] = $mod_query;
          $query = $mod_query;
        }
        else {
          $query = $this->modified_queries[$query];
        }
      }
      else {
        $xml->registerXPathNamespace($prefix, $namespace);
      }
    }

    list($results, $error) = $this->_query($xml, $query);

    if (is_object($error) && $error->level == LIBXML_ERR_ERROR) {
      $orig_query = array_search($query, $this->modified_queries);
      // If we didn't modify the query then it won't be in modified_queries.
      $orig_query = $orig_query ? $orig_query : $query;
      if ($this->source_config['exp']['errors']) {
       drupal_set_message(
        t("There was an error with the XPath query: %query.<br>
          Libxml returned the message: %message, with the error code: %code.",
          array('%query'   => $orig_query,
                '%message' => trim($error->message),
                '%code'    => $error->code)),
        'error',
        FALSE);
      }
    }
    if ($results === FALSE) {
      return array();
    }
    return $results;
  }

  private function debug($item, $source) {
    if (in_array($source, $this->debug)) {
      $o = '<ul>';
      foreach ($item as $i) {
        $o .= '<li>' . check_plain($i->asXML()) . '</li>';
      }
      $o .= '</ul>';
      drupal_set_message($source . ':' . $o);
    }
  }

  /**
   * Normalizes XPath queries, adding the default namespace.
   */
  private function addDefaultNamespace($query) {
    $parser = new FeedsXPathQueryParser($query);
    return $parser->getQuery();
  }

  /**
   * Parses one item from the context array.
   *
   * @param $item
   *   A  SimpleXMLElement from the context array.
   *
   * @param $query
   *   An XPath query.
   *
   * @param $source
   *   The name of the source for this query.
   *
   * @return array
   *   An array containing the results of the query.
   */
  protected function parseSourceElement($item, $query, $source) {

    if (empty($query)) {
      return;
    }
    $results = $this->namespacedXPathQuery($item, $query);
    unset($item);
    $this->debug($results, $source);
    /**
     * Iterate through the results of the xpath query.  If this source is
     * configured to return raw xml, make it so.
     */
    foreach ($results as &$result) {
      if (in_array($source, $this->rawXML)) {
        $result = $result->asXML();
      }
      else {
        $result = (string) $result;
      }
    }
    /**
     * If there is one result, return it directly.  If there are no results,
     * return.
     */
    if (count($results) === 1) {
      return $results[0];
    }
    if (count($results) === 0) {
      return;
    }
    return $results;
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = array();
    $form['#weight'] = -10;
    $form['#tree'] = TRUE;

    $mappings_ = feeds_importer($this->id)->processor->config['mappings'];
    $uniques = $mappings = array();
    foreach ($mappings_ as $mapping) {
      if (strpos($mapping['source'], 'xpathparser:') === 0) {
        $mappings[$mapping['source']] = $mapping['target'];
        if ($mapping['unique']) {
          $uniques[] = $mapping['target'];
        }
      }
    }

    if (empty($mappings)) {
      $form['error_message']['#value'] = 'FeedsXPathParser: No mappings are defined.<br>';
      return $form;
    }

    $form['context'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Context'),
      '#required'      => TRUE,
      '#description'   => t('This is the base query, all other queries will run in this context.'),
      '#default_value' => isset($source_config['context']) ? $source_config['context'] : '',
      '#maxlength'     => 1024,
    );

    $form['sources'] = array(
      '#type' => 'fieldset',
    );

    if (!empty($uniques)) {
      $items = array(
        format_plural(count($uniques),
          t('Field <strong>!column</strong> is mandatory and considered unique: only one item per !column value will be created.',
            array('!column' => implode(', ', $uniques))),
          t('Fields <strong>!columns</strong> are mandatory and values in these columns are considered unique: only one entry per value in one of these columns will be created.',
            array('!columns' => implode(', ', $uniques)))),
      );
      $form['sources']['help']['#value'] = '<div class="help">' . theme('item_list', $items) . '</div>';
    }

    $variables = array();
    foreach ($mappings as $source => $target) {
      $form['sources'][$source] = array(
        '#type'          => 'textfield',
        '#title'         => $target,
        '#description'   => t('The XPath query to run.'),
        '#default_value' => isset($source_config['sources'][$source]) ? $source_config['sources'][$source] : '',
        '#maxlength'     => 1024,
      );
      if (!empty($variables)) {
        $form['sources'][$source]['#description'] .= '<br>' . t('The variables '. implode(', ', $variables). ' are availliable for replacement.');
      }
      $variables[] = '$' . $target;
    }

    $form['rawXML'] = array(
      '#type'          => 'checkboxes',
      '#title'         => t('Select the queries you would like to return raw XML or HTML'),
      '#options'       => $mappings,
      '#default_value' => isset($source_config['rawXML']) ? $source_config['rawXML'] : array(),
    );

    $form['exp'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => 'XPath Parser Options',
    );

    $form['exp']['errors'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show error messages.'),
      '#default_value' => isset($source_config['exp']['errors']) ? $source_config['exp']['errors'] : FALSE,
    );

    if (extension_loaded('tidy')) {
      $form['exp']['tidy'] = array(
        '#type'          => 'checkbox',
        '#title'         => t('Use Tidy'),
        '#description'   => t('The Tidy PHP extension has been detected.
                              Select this to clean the markup before parsing.'),
        '#default_value' => isset($source_config['exp']['tidy']) ? $source_config['exp']['tidy'] : FALSE,
      );
    }

    $form['exp']['debug'] = array(
      '#type'    => 'checkboxes',
      '#title'   => t('Debug Query'),
      '#options' => array_merge(array('context' => 'context'), $mappings),
      '#default_value' => isset($source_config['exp']['debug']) ? $source_config['exp']['debug'] : array(),
    );

    return $form;
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = $this->sourceForm($this->config);
    $form['context']['#required'] = FALSE;
    return $form;
  }

  /**
   * Define defaults.
   */
  public function sourceDefaults() {
    return $this->config;
  }

  /**
   * Define defaults.
   */
  public function configDefaults() {
    return array(
      'sources' => array(),
      'rawXML'  => array(),
      'context' => '',
      'exp' => array(
        'errors' => FALSE,
        'tidy'   => FALSE,
        'debug'  => array(),
      ),
    );
  }

  /**
   * Override parent::sourceFormValidate().
   *
   * Simply trims all XPath values from the form. That way when testing them
   * later we can be sure that there aren't any strings with spaces in them.
   *
   * @todo
   *   validate xpath queries?
   *
   * @param &$values
   *   The values from the form to validate, passed by reference.
   */
  public function sourceFormValidate(&$values) {
    $values['context'] = trim($values['context']);
    foreach ($values['sources'] as &$query) {
      $query = trim($query);
    }
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function configFormValidate(&$values) {
    $this->sourceFormValidate($values);
  }

  /**
   * Here we set libxml_use_internal_errors to TRUE because depending on the
   * libxml version, $xml->xpath() might return FALSE or an empty array() when
   * a query doesn't match.
   */
  private function _query($xml, $query) {
    $use_errors = libxml_use_internal_errors(TRUE);

    // Perfom xpath query.
    $results = $xml->xpath($query);

    $error = libxml_get_last_error();
    libxml_clear_errors();
    libxml_use_internal_errors($use_errors);
    return array($results, $error);
  }

  /**
  * Override parent::getMappingSources().
  */
  public function getMappingSources() {
    $xpath_source = array(
      'xpathparser:0' => array(
        'name' => t('XPath Expression'),
        'description' => t('Allows you to configure an XPath expression that will populate this field.'),
      ),
    );

    $sources = parent::getMappingSources();
    // Older versions of Feeds return FALSE here.
    if (is_array($sources)) {
      return $sources + $xpath_source;
    }

    return $xpath_source;
  }
}

class FeedsXPathParserHTML extends FeedsXPathParserBase {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    parent::parse($batch, $source);

    $dom = new DOMDocument();
    $raw = trim($batch->getRaw());

    if (empty($raw)) {
      drupal_set_message(t('Feeds XPath parser: The HTML document is empty.'), 'warning');
      return;
    }
    /**
     * This is currently unsupported.
     */
    if ($this->source_config['exp']['tidy']) {
      $config = array(
        'merge-divs'       => FALSE,
        'merge-spans'      => FALSE,
        'join-styles'      => FALSE,
        'drop-empty-paras' => FALSE,
        'wrap'             => 0,
        'tidy-mark'        => FALSE,
        'escape-cdata'     => TRUE,
        'word-2000'        => TRUE,
      );
      $raw = tidy_repair_string($raw, $config);
    }
    if ($this->source_config['exp']['errors']) {
      $success = $dom->loadHTML($raw);
    }
    else {
      $success = @$dom->loadHTML($raw);
    }
    if (!$success) {
      throw new Exception(t('There was an error parsing the HTML document.'));
    }

    $xml = simplexml_import_dom($dom);
    unset($dom);
    $batch->setItems($this->query($xml));
  }
}

class FeedsXPathParserXML extends FeedsXPathParserBase {

  /**
   * Implementation of FeedsParser::parse().
   */
  public function parse(FeedsImportBatch $batch, FeedsSource $source) {
    parent::parse($batch, $source);
    $raw = trim($batch->getRaw());

    if (empty($raw)) {
      drupal_set_message(t('Feeds XPath parser: The XML document is empty.'), 'warning');
      return;
    }

    if ($this->source_config['exp']['tidy']) {
      $config = array(
        'input-xml' => TRUE,
        'wrap'      => 0,
        'tidy-mark' => FALSE,
      );
      $raw = tidy_repair_string($raw, $config);
    }

    $xml = new SimpleXMLElement($raw);
    $batch->setItems($this->query($xml));
  }
}


/**
 * Implementation of hook_form_feeds_ui_mapping_form_alter().
 *
 * This is an interesting bit of work. Each source name has to be unique,
 * but we have no idea how many to create with getMappingSources() because we
 * don't know how many targets there are going to be.
 *
 * The solution is to keep track in the form how many have been added.
 */
function feeds_xpathparser_form_feeds_ui_mapping_form_alter(&$form, &$form_state) {
  $newest_xpath_mapping = array();
  foreach ($form['#mappings'] as $mapping) {
    if (strpos($mapping['source'], 'xpathparser:') === 0) {
      $newest_xpath_mapping = $mapping;
    }
  }
  if (!empty($newest_xpath_mapping)) {
    list($a, $count) = explode(':', $newest_xpath_mapping['source']);
    $default_source = $a . ':' . '0';
    $label = $form['source']['#options'][$default_source];
    unset($form['source']['#options'][$default_source]);
    $form['source']['#options'][$a . ':' . ++$count] = $label;
  }
}


/**
 * Pseudo-parser of XPath queries.  When an XML document has a default
 * namespace this gets called so that adding the __default__ namepace where
 * appropriate. Aren't we nice?
 *
 * @todo
 *   Cleanup.
 * @param $query
 *   An xpath query string.
 * @return string
 *   An xpath query string with the __default__ namespace added.
 */
class FeedsXPathQueryParser {
  function __construct($query) {
    $this->query = preg_replace('/\s+\(\s*/', '(', $query);

    $this->word_boundaries = array(
      '[', ']', '=', '(', ')', '.', '<', '>', '*', '!', '|', '/', ',', ' ', ':',
    );

    $this->in_quotes = FALSE;
    $this->quote_char = '';
    $this->word = '';
    $this->output = '';
    $this->prev_boundary = '';
    $this->axis = '';
    $this->skip_next_word = FALSE;
    $this->start();
  }

  function start() {
    for ($i=0; $i < strlen($this->query); $i++) {
      $this->i = $i;
      $c = $this->query[$i];

      if ($c == '"' || $c == "'") {
        $this->handle_quote($c);
        continue;
      }

      if ($this->in_quotes) {
        $this->word .= $c;
        continue;
      }

      if (in_array($c, $this->word_boundaries)) {
        $this->handle_word_boundary($c);
      }
      else {
        $this->word .= $c;
      }
    }
    $this->handle_word();
  }

  function handle_quote($c) {
    if ($this->in_quotes && $c == $this->quote_char) {
      $this->in_quotes = FALSE;
      $this->word .= $c;
      $this->output .= $this->word;
      $this->word = '';
    }
    elseif (!$this->in_quotes) {
      $this->in_quotes = TRUE;
      $this->handle_word();
      $this->word = $c;
      $this->quote_char = $c;
    }
    else {
      $this->word .= $c;
    }
  }

  function handle_word_boundary($c) {
    if (in_array($this->word, array('div', 'or', 'and', 'mod')) &&
        $this->prev_boundary == ' ' && $c == ' ') {
      $this->output .= $this->word;
    }
    else {
      $this->handle_word($c);
    }
    $this->output .= $c;
    $this->word = '';
    $this->prev_boundary = $c;
  }

  function handle_word($c='') {
    if ($this->word == '') {
      return;
    }
    if ($c == ':' && $this->query[$this->i + 1] == ':') {
      $this->axis = $this->word;
    }
    if ($c == ':' && $this->query[$this->i-1] != ':'  &&
        $this->query[$this->i+1] != ':') {
      $this->output .= $this->word;
      $this->skip_next_word = TRUE;
      return;
    }

    if ($this->skip_next_word) {
      $this->skip_next_word = FALSE;
      $this->output .= $this->word;
      return;
    }

    if (is_numeric($this->word) ||
        $this->axis == 'attribute' ||
        strpos($this->word, '@') === 0 ||
        $c == '(' ||
        $c == ':') {
      $this->output .= $this->word;
      return;
    }

    $this->output .= '__default__:' . $this->word;
  }

  function getQuery() {
    return $this->output;
  }
}
