function VerticalPartition(data, elementID, configuration, events) {
    var self = this;

    this.data = data;
    this.configuration = configuration;
    this.events = events;

    var dimensions = getDimensions();

    var rect_render = new RectRender(dimensions.width,dimensions.height);


    var partition = d3.layout.partition()
    .value(function(node) {  return 1; });

    var element = d3.select('#'+elementID);
    var svg,rect;
    this.render = function() {
        element.html("");
        svg = element.append("svg")
        .attr("width", dimensions.width)
        .attr("height", dimensions.height)
        .append("g")
        .call(d3.behavior.zoom().on("zoom",  self.zoom));


        rect = svg.selectAll("rect")
        .data(partition.nodes(data))
        .enter().append("rect")
        .attr("x", rect_render.x)
        .attr("y", rect_render.y)
        .attr("width", rect_render.width)
        .attr("height", rect_render.height)
        .style("stroke", "#fff")
        .attr("fill",self.configuration.gradiante.normal)
        .on("click", self.click);
    }

    this.click = function(node) {
        self.events.click(node)
        rect_render.rectAnimation(rect,node);
    }

    this.resize = function(){
        dimensions = getDimensions();
        arc_render = new ArcRender(dimensions.width,dimensions.height);
        this.render();
    }

    this.zoom = function(){
        //alert('ok');
        if (d3.event) {
            svg.attr("transform", "translate(" + d3.event.translate + ")"+"scale(" + d3.event.scale + ")");
        }
    }

}


function RectRender(width,height){
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
        .duration(self.configuration.currentConfig.animationTransitionTime)
        .attr("x", function(node) { return x(node.x); })
        .attr("y", function(node) { return y(node.y); })
        .attr("width", function(node) { return x(node.x + node.dx) - x(node.x); })
        .attr("height", function(node) { return y(node.y + node.dy) - y(node.y); });
    }
}


function verticalInit(elementID,width,height){
    var vertical = new VerticalPartition(data_ex[0],d3.select(elementID),width,height,calculateColor,visClickEv);
    vertical.render();
}