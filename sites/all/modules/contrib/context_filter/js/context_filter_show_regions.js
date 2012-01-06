$(document).ready(function() {
  // Find the right text for the toggle link.
  var text = Drupal.t('Show empty regions');
  if (Drupal.settings.context_filter.isShown) {
    text = Drupal.t('Hide empty regions');
  }

  // Add toggle regions link.
  var item = '<li><div class="label">' + text + '</div><div class="links"><a class="context-filter-links context-filter-show-regions" href="#regions">' + Drupal.t('Toggle') +'</a></div></li>';
  $('#context-ui-editor .context-editor ul').append(item);
  $('.context-filter-show-regions').click(function(e){
    $.get('/context_filter/regions', function() {
      // Reload the page to toggle the empty regions.
      window.location.reload();
    });
    e.preventDefault();
    return false;
  });
});
