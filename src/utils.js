function visClickEv(node) {
	alert(node);
}

function calculateColor(d) {
	if ( typeof calculateColor.d3Color == 'undefined' ) {
        calculateColor.d3Color = d3.scale.category20c();
    }
	return calculateColor.d3Color((d.children ? d : d.parent).name); 
}