var ZoomController_HTML;
function ZoomController(elementSel,zoomElement,svg,configuration){
    var eventsBlocked = false;
    var zoomListener = d3.behavior.zoom().scaleExtent([1, 10]).on("zoom", 
        function(){
            if (d3.event) {
                d3.event.zoomed = true;
                svg.attr("transform", "translate(" + d3.event.translate + ")"+"scale(" + d3.event.scale + ")");
            }
        });

    var zoomStack = [];


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
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },
    down: function(){
        if(eventsBlocked)
            return;
        eventsBlocked = true;
        var curTranslate = zoomListener.translate();
        curTranslate[1] -= 100;
        zoomListener.translate(curTranslate);
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },
    right: function(){
        if(eventsBlocked)
            return;
        eventsBlocked = true;
        var curTranslate = zoomListener.translate();
        curTranslate[0] -= 100;
        zoomListener.translate(curTranslate);
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },
    left: function(){
        if(eventsBlocked)
            return;
        eventsBlocked = true;
        var curTranslate = zoomListener.translate();
        curTranslate[0] += 100;
        zoomListener.translate(curTranslate);
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },            
    zoomIn: function(){
        if(eventsBlocked)
            return;
        eventsBlocked = true;
        var curScale = zoomListener.scale();
        ++curScale
        if(curScale <= 10){
            zoomListener.scale(curScale);
            zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
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
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },
    zoomReset: function(){
        //if(eventsBlocked)
         //   return;
        //eventsBlocked = true;
        zoomListener.scale(1);
        zoomListener.translate([0,0]);
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },

    zoomSave: function(){
        zoomStack.push([zoomListener.translate(),zoomListener.scale()]);
    },

    zoomRecover: function(){
        var lastZoom = zoomStack.pop();
        if(lastZoom != null){
            zoomListener.translate(lastZoom[0]);
            zoomListener.scale(lastZoom[1]);
            zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
        }
    },

    setZoom: function(zoom){
        zoomListener.translate(zoom[0]);
        zoomListener.scale(zoom[1]);
        zoomListener.event(zoomElement.transition().duration(configuration.currentConfig.zoomAnimationTime).each("end", eventsUnlock));
    },

    getZoom: function(){
        return [zoomListener.translate(),zoomListener.scale()];
    },

    zoomBlock: function(){
        eventsBlocked = true;
    },

    zoomUnlock: function(){
        eventsBlocked = false;
    }

};


zoomListener(zoomElement);
zoomElement.on("dblclick.zoom", null)
zoomListener.event(zoomElement);
zoomElement.on("mousedown", function() { d3.event.preventDefault(); });

$('#zoomContainer').remove();
$(elementSel).prepend(ZoomController_HTML);
$('#panUp').click(events.up);
$('#panDown').click(events.down);
$('#panLeft').click(events.left);
$('#panRight').click(events.right);
$('#zoomIn').click(events.zoomIn);
$('#zoomOut').click(events.zoomOut);
$('#zoomReset').click(events.zoomReset);
$(document).click(function(e){
  // checking for any non left click and convert to left click.
  if (e.which == 2) { 
    events.zoomReset();
}
});



return events;
}

function multiline(f) {
  return f.toString().
      replace(/^[^\/]+\/\*!?/, '').
      replace(/\*\/[^\/]+$/, '');
}


ZoomController_HTML = multiline(function() {/*!
      <div id="zoomContainer" class="leaflet-control-container">
        <div id="zoomInside">
        <div class="leaflet-top leaflet-left has-leaflet-pan-control">
          <div class="leaflet-control-pan leaflet-control">
            <div class="leaflet-control-pan-up-wrap">
              <a id="panUp" class="leaflet-control-pan-up" title="Up"></a>
            </div>
            <div class="leaflet-control-pan-left-wrap">
              <a id="panLeft" class="leaflet-control-pan-left" title="Left"></a>
            </div>
            <div class="leaflet-control-pan-right-wrap">
              <a id="panRight" class="leaflet-control-pan-right" title="Right"></a>
            </div>
            <div class="leaflet-control-pan-down-wrap">
              <a id="panDown" class="leaflet-control-pan-down" title="Down"></a>
            </div>

            <div class="leaflet-control-pan-center-wrap">
              <a id="zoomReset" class="leaflet-control-pan-center" title="Left"></a>
            </div>
          </div>
          <div class="leaflet-control-zoom leaflet-bar leaflet-control">
            <a id="zoomIn" class="leaflet-control-zoom-in" title="Zoom in">+</a>
            <a id="zoomOut" class="leaflet-control-zoom-out" title="Zoom out">-</a>
          </div>
        </div>
      </div>
      </div>
*/});