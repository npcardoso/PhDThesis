var ZoomController_HTML;
function ZoomController(elementSel,events){
	$(elementSel).html(ZoomController_HTML);
	$('#panUp').click(events.up);
	$('#panDown').click(events.down);
	$('#panLeft').click(events.left);
	$('#panRight').click(events.right);
	$('#zoomIn').click(events.zoomIn);
	$('#zoomOut').click(events.zoomOut);
}

$.ajax({
	url: 'components/zoom-control/zoomcontrol.html',
	type: 'get',
	dataType: 'html',
	async: false,
	success: function(html) {
		ZoomController_HTML = html;
                }
            });