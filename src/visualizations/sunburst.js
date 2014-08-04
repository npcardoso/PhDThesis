function Sunburst(data, element, width, height, colorFunction, clickEvent) {
    var self = this;

    this.data = data;
    this.element = element;
    this.color = colorFunction;
    this.clickEvent = clickEvent;  
    
    var arc_render = new ArcRender(width,height);
    var svg, path;

    //Partion layout preparion (Create of the function that computes elements relative size)
    var partition = d3.layout.partition()
    .value(function(d) {
        return 1;
    });

	//Public rendering function renders the visualion on the element passed
    this.render = function() {
        element.html("");
        svg = element.append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + width / 2 + "," + (height / 2 + 10) + ")");

        path = svg.selectAll("path")
        .data(partition.nodes(self.data))
        .enter().append("path")
        .attr("d", arc_render.arc)
        .style("stroke", "#fff")
        .style("fill", self.color)
        .style("fill-rule", "evenodd")
        .on("click", self.click);
    };

	//Function called when a node is clicked call the click event and applicates the animation
    this.click = function(node) {
        self.clickEvent(node);
        path.transition()
        .duration(750)
        .attrTween("d", arc_render.arcTween(node));
    }
}


function ArcRender(width,height){
    var radius = Math.min(width, height) / 2;

    var x = d3.scale.linear()
    .range([0, 2 * Math.PI]);

    var y = d3.scale.sqrt()
    .range([0, radius]);

    //Returns Function that computes an svg arc for specied dimensions
    var arc = d3.svg.arc()
    .startAngle(function(d) {
        return Math.max(0, Math.min(2 * Math.PI, x(d.x)));
    })
    .endAngle(function(d) {
        return Math.max(0, Math.min(2 * Math.PI, x(d.x + d.dx)));
    })
    .innerRadius(function(d) {
        return Math.max(0, y(d.y));
    })
    .outerRadius(function(d) {
        return Math.max(0, y(d.y + d.dy));
    });
    this.arc = arc;

    //Computes the arc animation
    this.arcTween =   function(node) {
        var xd = d3.interpolate(x.domain(), [node.x, node.x + node.dx]),
        yd = d3.interpolate(y.domain(), [node.y, 1]),
        yr = d3.interpolate(y.range(), [node.y ? 20 : 0, radius]);
        return function(node, i) {
            return i ? function(t) {
                return arc(node);
            } : function(t) {
                x.domain(xd(t));
                y.domain(yd(t)).range(yr(t));
                return arc(node);
            };
        };
    }
}

function sunburstInit(elementID,width,height){
    var sunburst = new Sunburst(data_ex[0],d3.select(elementID),width,height,calculateColor,visClickEv);
    sunburst.render();
}