/**
 * @file
 * Prevent use of ingest/next (page submit) while a file is uploading (upload.form.inc).
 */

(function($) {
    Drupal.behaviors.lib4ridora = {
        attach: function (context, settings) {
            // Before file is uploaded disable the Ingest or next button so the
            // user cannot submit until all files are uploaded.
            $('form').find('.plupload-element').each( function(index){
                var uploader = $(this).pluploadQueue();
                uploader.bind('BeforeUpload', function(up, file) {
                    // Called right before the upload for a given file starts.
                    $('input[value="Ingest"]').attr('disabled', 'disabled');
                    $('input[value="Next"]').attr('disabled', 'disabled');
                });
            });
            // Triggers after the ajax for processing the files and enables the
            // ingest or submit button.
            $('#lib4ridora_upload_form').ajaxComplete(function(){
                $('input[value="Ingest"]').removeAttr('disabled');
                $('input[value="Next"]').removeAttr('disabled');
            });
        }
    }
})(jQuery);
