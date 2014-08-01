//Program entry point
function init(){
	//Calculate rendering dimensions (dependent on css)
	var width= $(window).width()-75;
	var height = $(window).height()-115;

	//Render html of the buttons
	$("body").append('<div id="tabs"><ul></ul></div>');
	$.each(visualizations, function(index, visualization) {
		$("#tabs ul").append('<li><a href="#tabs-'+index+'">'+visualization.displayName+'</a></li>');
		$("#tabs").append('<div id="tabs-'+index+'"></div>');
	});

	$( "#tabs" ).tabs({
		activate: function( event, ui ) {
			var visualizationIndex = ui.newTab.children("a").attr("href").replace("#tabs-","");
			visualizations[visualizationIndex].init("#tabs-"+visualizationIndex,width,height);
		}
	});

	//Render the default visualizations
	visualizations[0].init("#tabs-0",width,height);
}

//Function called when a node is clicked on the visualization
function visClickEv(node) {
	alert(node);
}

//Helper function to calculate the color of a node
function calculateColor(d) {
	if ( typeof calculateColor.d3Color == 'undefined' ) {
		calculateColor.d3Color = d3.scale.category20c();
	}
	return calculateColor.d3Color((d.children ? d : d.parent).name); 
}
