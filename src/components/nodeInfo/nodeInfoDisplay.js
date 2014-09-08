function initializeBreadcrumbTrail(elementSel) {


  $('#breadcrumbs').remove();
  $('#trail').remove();
  $('#endlabel').remove();

    d3.select(elementSel).append("div")
  .attr("id", "breadcrumbs")

  // Add the label at the end, for the percentage.
  d3.select(elementSel).append("div")
  .attr("id", "endlabel");
}


// Update the breadcrumb trail to show the current sequence and percentage.
function updateBreadcrumbs(nodeArray, clickFunction, configuration) {
  var last = nodeArray.slice(-1)[0];

  var color = configuration.gradiante.normal(last);
  // Now move and update the percentage at the end.
  var percentage, displayText;
  if(last.score >= 0){
      percentage = d3.round(last.score*100,2);
      displayText = percentage+'%';
  }
  else
  {
    percentage = 0;
    displayText = 'Uncalculated';
  }
 $("#endlabel").html('<div class="pace pace-active" style="border-color: '+color+';"><div class="pace-progress" data-progress="'+percentage+'" data-progress-text="'+displayText+'" style="width: '+percentage+'%;   background: '+color+'; color: '+color+';"><div class="pace-progress-inner"></div></div><div class="pace-activity"></div></div>');
  

 $('#breadcrumbs').html('<ol class="breadcrumbs"></ol>');

 var lengthN = nodeArray.length;
 for (var i = 0; i < lengthN; i++) {
   $('.breadcrumbs').append('<li><a href="#" id="node-'+i+'"><span>'+nodeArray[i].n+'</span></a></li>');
   var x = $('body').append('<style>.breadcrumbs li:nth-child('+(i+1)+') a, .breadcrumbs li:nth-child('+(i+1)+') a:before, .breadcrumbs li:nth-child('+(i+1)+') a:after{background-color: '+configuration.gradiante.normal(nodeArray[i])+';}</style>');
    $('.breadcrumbs a').click(function(){
      var id = $(this).attr('id').replace("node-", ""); 
      clickFunction(nodeArray[id]);
    })
 };

   

 

  // Make the breadcrumb trail visible, if it's hidden.


}



function NodeInfoDisplay(elementSel,clickFunction,configuration){
  var self = this;
  initializeBreadcrumbTrail(elementSel);

  this.updataBreadcumb = function(node){
    updateBreadcrumbs(getAncestors(node),clickFunction,configuration);
  }

  this.setClicked = function(node){
    self.clickedNode = node;
    self.updataBreadcumb(node);
  }

  this.setPath = function(path){
    self.path = path;
  }

  this.mouseover = function(node){
    self.updataBreadcumb(node);
    var sequenceArray = getAncestors(node);
    self.path.style("opacity", 0.3);

    self.path.filter(function(node) {
      return (sequenceArray.indexOf(node) >= 0);
    })
    .style("opacity", 1);
  }


  this.mouseleave = function(node){
    self.updataBreadcumb(self.clickedNode);
        //path.on("mouseover", null);
        self.path.style("opacity", 1);
        return;
        self.path.transition()
        .duration(1000)
        .style("opacity", 1)
        .each("end", function() {
          d3.select(this).on("mouseover", self.mouseover);
        });
      }

    }