function init(){
	var width= $(window).width()-75;
	var height = $(window).height()-115;

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

	visualizations[0].init("#tabs-0",width,height);
}
