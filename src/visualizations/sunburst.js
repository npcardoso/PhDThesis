function Sunburst(data, element, width, height, colorFunction, clickEvent) {	
    this.data = data;
    this.element = element;
    this.color = colorFunction;
    this.clickEvent = clickEvent;  
    
    var svg, path;
	//Public rendering function renders the visualion onthe element passed
    this.render = function() {
        element.html("");
        svg = element.append("svg")
            .attr("width", width)
            .attr("height", height)
            .append("g")
            .attr("transform", "translate(" + width / 2 + "," + (height / 2 + 10) + ")");


        path = svg.selectAll("path")
            .data(partition.nodes(this.data))
            .enter().append("path")
            .attr("d", arc)
            .style("stroke", "#fff")
            .style("fill", this.color)
           	.style("fill-rule", "evenodd")
            .on("click", click);
    };

	//Partion layout preparion (Create of the function that computes elements relative size)
	var partition = d3.layout.partition()
        .value(function(d) {
            return 1;
     });
    
    
    //Arc rendering function and subfunctions preparation
    var radius = Math.min(width, height) / 2;

    var x = d3.scale.linear()
        .range([0, 2 * Math.PI]);

    var y = d3.scale.sqrt()
        .range([0, radius]);

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
	
	//Function called when a node is clicked call the clicke event and applites the animation
     function click(d) {
        clickEvent(d);
           path.transition()
               .duration(750)
               .attrTween("d", arcTween(d));
    }
    // Interpolate the scales! Calculates the arc size by time (used by animation)
    function arcTween(d) {
        var xd = d3.interpolate(x.domain(), [d.x, d.x + d.dx]),
            yd = d3.interpolate(y.domain(), [d.y, 1]),
            yr = d3.interpolate(y.range(), [d.y ? 20 : 0, radius]);
        return function(d, i) {
            return i ? function(t) {
                return arc(d);
            } : function(t) {
                x.domain(xd(t));
                y.domain(yd(t)).range(yr(t));
                return arc(d);
            };
        };
    }
}

function sunburstInit(elementID,width,height){
    var sunburst = new Sunburst(data_ex[0],d3.select(elementID),width,height,calculateColor,visClickEv);
    sunburst.render();
}