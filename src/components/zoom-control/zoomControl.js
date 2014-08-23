var ZoomController_HTML;
function ZoomController(elementSel,zoomElement,svg,configuration){
    var zoomListener = d3.behavior.zoom().scaleExtent([1, 10]).on("zoom", 
        function(){
            if (d3.event) {
                svg.attr("transform", "translate(" + d3.event.translate + ")"+"scale(" + d3.event.scale + ")");
            }
        });

    var eventsBlocked = false;
    function eventsUnlock(){
        eventsBlocked = false;
    }
    var events = {up: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                var curTranslate = zoomListener.translate();
                curTranslate[1] += 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
            },
            down: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                var curTranslate = zoomListener.translate();
                curTranslate[1] -= 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
            },
            right: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                var curTranslate = zoomListener.translate();
                curTranslate[0] -= 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
            },
            left: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                var curTranslate = zoomListener.translate();
                curTranslate[0] += 100;
                zoomListener.translate(curTranslate);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
            },            
            zoomIn: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                var curScale = zoomListener.scale();
                ++curScale
                if(curScale <= 10){
                    zoomListener.scale(curScale);
                    zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
                }
                eventsUnlock();
            },
            zoomOut: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                var curScale = zoomListener.scale();
                --curScale;
                if(curScale < 1){
                    curScale = 1;
                }
                zoomListener.scale(curScale);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
            },
            zoomReset: function(){
                if(eventsBlocked)
                    return;
                eventsBlocked = true;
                zoomListener.scale(1);
                zoomListener.translate([0,0]);
                zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.animationTransitionTime).each("end", eventsUnlock));
            }
        };

        zoomListener(zoomElement);
        zoomListener.event(zoomElement);

        $('#zoomContainer').remove();
        $(elementSel).prepend(ZoomController_HTML);
        $('#panUp').click(events.up);
        $('#panDown').click(events.down);
        $('#panLeft').click(events.left);
        $('#panRight').click(events.right);
        $('#zoomIn').click(events.zoomIn);
        $('#zoomOut').click(events.zoomOut);
        $('#zoomReset').click(events.zoomReset);
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