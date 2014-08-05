function VerticalPartition(data, element, width, height, colorFunction, clickEvent) {
    var self = this;

    this.data = data;
    this.element = element;
    this.color = colorFunction;
    this.clickEvent = clickEvent;  

    var x = d3.scale.linear()
    .range([0, width]);

    var y = d3.scale.linear()
    .range([0, height]);


    var partition = d3.layout.partition()
    .value(function(node) {  return 1; });

    var svg,rect;
    this.render = function() {
        element.html("");
        svg = element.append("svg")
        .attr("width", width)
        .attr("height", height);


        rect = svg.selectAll("rect")
        .data(partition.nodes(data))
        .enter().append("rect")
        .attr("x", function(d) { return x(d.x); })
        .attr("y", function(d) { return y(d.y); })
        .attr("width", function(d) { return x(d.dx); })
        .attr("height", function(d) { return y(d.dy); })
        .style("stroke", "#fff")
        .attr("fill",self.color)
        .on("click", self.click);
    };

    this.click = function(node) {
        self.clickEvent(node)
        x.domain([node.x, node.x + node.dx]);
        y.domain([node.y, 1]).range([node.y ? 20 : 0, height]);

        rect.transition()
        .duration(750)
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