function SunburstNoAni(data, element, width, height, colorFunction, clickEvent) {	
    this.data = data;
    this.element = element;
    this.color = colorFunction;
    this.clickEvent = clickEvent;  

    var radius = Math.min(width, height) / 2;

    var partition = d3.layout.partition()
    .sort(null)
    .size([2 * Math.PI, radius * radius])
    .value(function(d) { return 1; });

    var arc = d3.svg.arc()
    .startAngle(function(d) { return d.x; })
    .endAngle(function(d) { return d.x + d.dx; })
    .innerRadius(function(d) { return Math.sqrt(d.y); })
    .outerRadius(function(d) { return Math.sqrt(d.y + d.dy); });

    var svg,path;
    this.render = function(){
        element.html("");
        svg = element.append("svg")
        .attr("width", width)
        .attr("height", height)
        .append("g")
        .attr("transform", "translate(" + width / 2 + "," + height * .52 + ")");
        
        render(data);
    }

    function render(d){
      svg.selectAll("path").remove();
      path = svg.selectAll("path")
      .data(partition.nodes(d).filter(function(d) {
      return (d.dx > 0.005); // 0.005 radians = 0.29 degrees
      }))
      .enter().append("path")
      .attr("display", function(d) { return d.depth ? null : "none"; }) // hide inner ring
      .attr("d", arc)
      .style("stroke", "#fff")
      .style("fill", colorFunction)
      .style("fill-rule", "evenodd")
      .on("click", click);
  }

  function click(d){
    clickEvent(d);
    render(d);
  }

}

function sunburstNoAniInit(elementID,width,height){
    var sunburst = new SunburstNoAni(data_ex[0],d3.select(elementID),width,height,calculateColor,visClickEv);
    sunburst.render();
}