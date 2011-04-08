$(document).ready(function() {

  function moveCloseLink() {
    // Set variables
    var sbClose = $('#sb-nav-close');
    var sbTitle = $('#sb-title');

    // Add the title from the close link to Drupal's t() function
    sbClose.attr('title',Drupal.t('Close'));

    if(sbTitle) {
      // Add a span to sbTitle with Shadowbox close link
      // Don't add the text directly to the <a> tag, there is a IE7/8 issue with "filter: .. AlphaImageLoader .."
      sbTitle.append('<span id="sb-nav-label">' + sbClose.attr('title') + '</span>').click(function() {Shadowbox.close()});
      // Append sbTitle
      sbTitle.append(sbClose);
    }
  }

  function hideFlashElements() {
    $('#page iframe,#page embed').hide();
  }

  function showFlashElements() {
    $('#page iframe,#page embed').show();
  }

  function shadowboxOpen() {
    moveCloseLink();
    hideFlashElements()
  }

  // Call the function when Shadowbox opens
  Shadowbox.options.onOpen = shadowboxOpen;

  // Call the function when Shadowbox closes
  Shadowbox.options.onClose = showFlashElements;

});