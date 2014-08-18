var ZoomController_HTML;
function ZoomController(elementSel,zoomListener,zoomElement,configuration){
	var events = {up: function(){
                //alert('ok');
                var curTranslate = zoomListener.translate();
                curTranslate[1] -= 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime));
            },
            down: function(){
                //alert('ok');
                var curTranslate = zoomListener.translate();
                curTranslate[1] += 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime));
            },
            right: function(){
                //alert('ok');
                var curTranslate = zoomListener.translate();
                curTranslate[0] += 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime));
            },
            left: function(){
                //alert('ok');
                var curTranslate = zoomListener.translate();
                curTranslate[0] -= 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime));
            },            
            zoomIn: function(){
                var curScale = zoomListener.scale();
                ++curScale
                if(curScale <= 10){
                zoomListener.scale(curScale);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime));
                }
            },
            zoomOut: function(){
                var curScale = zoomListener.scale();
                --curScale;
                if(curScale < 1){
                    curScale = 1;
                }
                zoomListener.scale(curScale);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime));
            }
        };
	$(elementSel).prepend(ZoomController_HTML);
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