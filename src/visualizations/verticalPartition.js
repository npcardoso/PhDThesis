function VerticalPartition(data, elementSel, configuration, events) {
    var self = this;
    data = data[0];

    this.data = data;
    this.configuration = configuration;
    this.events = events;

    var dimensions = getDimensions();

    var rect_render = new RectRender(dimensions.width,dimensions.height,configuration);


    var partition = d3.layout.partition()
    .value(function(node) {  return 1; });


    var element = d3.select(elementSel);
    var svg,rect;


    this.zoomEvents = null;
    this.clicked = data;
    this.stateManager = new StateManager(self);
    this.render = function() {
        element.html("");

        self.nodeInfoDisplay = new NodeInfoDisplay(elementSel,self.dblclick,configuration);
        self.stateManager.initRender(elementSel);

        var zoomElement = element.append("svg")
        .attr("width", dimensions.width)
        .attr("height", dimensions.height)
        .append("g");

        svg = zoomElement.append("g");


        rect = svg.selectAll("rect")
        .data(partition.nodes(data))
        .enter().append("rect")
        .attr("x", rect_render.x)
        .attr("y", rect_render.y)
        .attr("width", rect_render.width)
        .attr("height", rect_render.height)
        .style("stroke", "#fff")
        .attr("fill",configuration.gradiante.normal)
        .on("dblclick", self.dblclick)
        .on("click", self.click)
        .on("mouseover", self.nodeInfoDisplay.mouseover)
        .on("mouseleave", self.nodeInfoDisplay.mouseleave);

        self.nodeInfoDisplay.setClicked(data);
        self.nodeInfoDisplay.setPath(rect);
        self.zoomEvents = ZoomController(elementSel,zoomElement,svg,self.configuration);
        keyBindings(self,configuration);
    }

    var isClicking = false;
    this.dblclick = function(node,noStateSAndZoomReset) {
        if(isClicking)
            return false;
        isClicking = true;
        if(noStateSAndZoomReset){
            self.stateManager.saveState();
            self.zoomEvents.zoomReset();
        }
        self.nodeInfoDisplay.setClicked(node);
        events.click(node)
        rect_render.rectAnimation(rect,node).each("end",function(){
            isClicking = false;
        });

        self.clicked = node;
    }

    this.click = function(node){
        if(d3.event.hasOwnProperty('zoomed')) return;
        events.click(node);
        console.log(node);
    }

    this.resize = function(){
        dimensions = getDimensions();
        arc_render = new ArcRender(dimensions.width,dimensions.height,self.configuration);
        this.render();
    }

    this.zoom = function(){
        //alert('ok');
        if (d3.event) {
            svg.attr("transform", "translate(" + d3.event.translate + ")"+"scale(" + d3.event.scale + ")");
        }
    }

}


function RectRender(width,height,configuration){
    var x = d3.scale.linear()
    .range([0, width]);

    var y = d3.scale.linear()
    .range([0, height]);

    this.x = function(node){
        return x(node.x);
    }

    this.y = function(node){
        return y(node.y);
    }

    this.width = function(node){
        return x(node.dx);
    }

    this.height = function(node){
        return y(node.dy);
    }

    this.rectAnimation = function(rect,node){
        x.domain([node.x, node.x + node.dx]);
        y.domain([node.y, 1]).range([node.y ? 20 : 0, height]);

       return rect.transition()
        .duration(configuration.currentConfig.animationTransitionTime)
        .attr("x", function(node) { return x(node.x); })
        .attr("y", function(node) { return y(node.y); })
        .attr("width", function(node) { return x(node.x + node.dx) - x(node.x); })
        .attr("height", function(node) { return y(node.y + node.dy) - y(node.y); });
    }
}
