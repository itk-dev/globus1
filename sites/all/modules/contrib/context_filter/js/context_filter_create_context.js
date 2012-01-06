$(document).ready(function() {
  // Add toggle regions link.
  var item = '<li><div class="label">' + Drupal.t('Current page') + '</div><div class="links"><a class="context-filter-links context-filter-create-context" href="#create_context">' + Drupal.t('Create') +'</a></div></li>';
  $('#context-ui-editor .context-editor ul').append(item);
  $('.context-filter-create-context').click(function(e){
    var settings = Drupal.settings.context_filter;
    $.get('/context_filter/create', {path : settings.currentPath}, function() {
      window.location.reload();
    });
    e.preventDefault();
    return false;
  });
});