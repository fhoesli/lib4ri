/**
 * @file
 * Interfere with islandora_authority
 */

(function($) {
  /**
   * Attaches the copyfield behaviour to all required fields
   */
  Drupal.behaviors.lib4ridora_authority = {
    attach: function(context, settings) {
      const COPY_ATTR_NAME = 'copy_from_islandora_authority_hidden_name'; // the attribute that needs to be defined to trigger copying
      const SHOW_ATTR_NAME = 'show_from_islandora_authority_hidden_name'; // the attribute that needs to be defined to trigger popup only
      // we observe changes to the islandora_authority_hidden field via MutationObserver
      var lib4ridora_authority_observer = new MutationObserver(function(mutations) {
        mutations.forEach(function(mutation) {
          if (mutation.type == 'attributes') {
            $(mutation.target).trigger('change'); // we define a change-handler below
          }
        });
      });
      $('html > head').append($('<style>[role="tooltip"] { display:none; } input:focus ~ [role="tooltip"].aria-tooltip-visible, [role="tooltip"].aria-tooltip-visible:hover { display: block; position: absolute; top: -20%; padding: 0 3px; background: #e0e0e0; border: 1px solid #707070; white-space: nowrap; overflow: hidden; }</style>'));
      $('input.islandora-authority-autocomplete', context).once('lib4ridora-authority', function() {
        var thisuri = this.value;
        var thisid = this.id.substr(0, this.id.length - 34);
        var thisid_mainpart = thisid.split('--').slice(0, 1);
        var $thisel = $('#' + thisid); // we assume we have unique ids...
        if ($thisel.length != 1) { return; } // abort, if ids are not unique
        var thisname = $thisel.attr('name');
        var thisname_mainpart = thisname.replace(/\[[^\]]*\]$/, '');
        // the following instruction solves the bug in islandora_authority that emptying autocomplete fields does not empty related fields (especially the hidden ones)
        $thisel.bind("change", function() { // nuke related fields if all visible autocomplete fields are emptied
          if ($thisel.val() === "") {
            $targets = $('input[id^=\'' + thisid_mainpart + '\'][name^=\'' + thisname_mainpart + '\'][name!=\'' + thisname + '\'][value!=\'\']');
            if ($targets.filter("[type='text'][value!='']").length == 0) {
              $targets.val("");
              $targets.trigger('change'); // trigger the change event on the other fields so that validation can take place
            }
          }
        });
        var copyval = true;
        var attr_name_val = $thisel.attr(COPY_ATTR_NAME);
        if (attr_name_val == undefined) {
          copyval = false;
          attr_name_val = $thisel.attr(SHOW_ATTR_NAME);
        }
        if (attr_name_val != undefined) {
          var $tooltipid = $thisel.attr('id') + '-aria-tooltip';
          $thisel.attr('aria-describedby', $tooltipid);
          $thisel.after($('<div role="tooltip"></div>').attr('id', $tooltipid).css('width', $thisel.css('width')).text("I am tooltip " + $tooltipid));
          // select the "cousin" islandora_authority_hidden element from which to copy
          var sourcename_pre = thisname.replace(/\[[^\]]*\]\[[^\]]*\]$/, ''); // nuke the last two selectors
          var sourcename_suf = '[' + attr_name_val + ']'; // define the last selector
          var $sourceel = $('input[name^=\'' + sourcename_pre + '\'][name$=\'' + sourcename_suf + '\']')
          if ($sourceel.length != 1) { return; } // abort, if name is not unique
          // NOTE: if we want to make islandora_authority_textfield fields
          // copyable, we would have to modify $.fn.val() so that setting
          // values of those fields via .val(newvalue) triggers
          // .setAttribute('value', newvalue)
          // Apparently, this is due to the fact that hidden fields change
          // the DOM when .val() is invoked, whereas textfields do not, so
          // that our observer only catches changes in the former case [TO
          // BE CONFIRMED]...
          // If this were to be implemented, we suggest to define an array
          // with all the $targetel ids on which the re-definition of
          // .val() should act. E.g., start the Drupal-attached function with
          ////var $ids_alter_val = [];
          ////var oVf = $.fn.val;
          ////$.fn.val = function() {
          ////  var result = oVf.apply(this, arguments);
          ////  if (arguments.length > 0 && $.inArray(this.attr('id'), $ids_alter_val) != -1) {
          ////    $(this)[0].setAttribute('value', oVf.apply(this));
          ////  }
          ////  return result;
          ////};
          // and include the following here
          ////$sourceel.each(function(index){
          ////  var id = $(this).attr('id');
          ////  if (id != "" && $('#' + id).length == 1) { // only act on unique ids
          ////  $ids_alter_val.push($(this).attr('id'));
          ////  }
          ////});
          $sourceel.change(function() { // add the change-handler to the islandora_authority_hidden field from which we want to copy when it gets autocompleted
            if ($sourceel.val() != "") {
              $('#' + $tooltipid).addClass('aria-tooltip-visible').text($sourceel.val());
            }
            else {
              $('#' + $tooltipid).removeClass('aria-tooltip-visible').text("");
            }
            if (!copyval) {
              return;
            }
            $thisel.val($sourceel.val()); // copy the current value
            $.ajax({ // now we autocomplete the other form
              'url': thisuri + '/' + $thisel.val(),
              'dataType': 'json',
              'success': function(data, textStatus, jqXHR) {
                if (Object.keys(data).length != 1){
                  $thisel.val("");
                  $thisel.trigger('change');
                }
                else {
                  var parents = $thisel[0].id.split('--');
                  var id_parts = parents.slice(0, parents.length -1);
                  var obj = data[Object.keys(data)[0]]; // get the only item found
                  var obj_properties = Object.getOwnPropertyNames(obj);
                  obj_properties.forEach(function(name) {
                    if (typeof obj[name] !== 'function' && name != 'full-display' && name != 'alts') {
                      id_parts.push(name.replace(/(\]\[|_| )/g, '-'));
                      $('#' + id_parts.join('--')).val(obj[name]); // copy the relevant value in the field
                      $('#' + id_parts.join('--')).trigger('change'); // trigger the change event so that validation can take place
                      id_parts.pop();
                    }
                  });
                }
              }
            });
          });
          if ($sourceel[0] != undefined){
            lib4ridora_authority_observer.observe($sourceel[0], {attributes: true, attributeFilter: ['value']});
          }
        }
      });
    }
  };
})(jQuery);
