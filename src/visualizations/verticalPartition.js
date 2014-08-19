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

    var clickedNode  = data;

    var element = d3.select(elementSel);
    var svg,rect;
    this.render = function() {
        element.html("");

        self.nodeInfoDisplay = new NodeInfoDisplay(elementSel,clickedNode,self.click,configuration);

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
        .on("click", self.click)
        .on("mouseover", self.nodeInfoDisplay.mouseover)
        .on("mouseleave", self.nodeInfoDisplay.mouseleave);

        self.nodeInfoDisplay.setPath(rect);
        ZoomController(elementSel,zoomElement,svg,self.configuration);
    }

    this.click = function(node) {
        self.nodeInfoDisplay.updataBreadcumb(node);
        events.click(node)
        rect_render.rectAnimation(rect,node);
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

        rect.transition()
        .duration(configuration.currentConfig.animationTransitionTime)
        .attr("x", function(node) { return x(node.x); })
        .attr("y", function(node) { return y(node.y); })
        .attr("width", function(node) { return x(node.x + node.dx) - x(node.x); })
        .attr("height", function(node) { return y(node.y + node.dy) - y(node.y); });
    }
}
