function dataInlining (data) {
	for (var i = data.length - 1; i >= 0; i--) {
		if(data[i].hasOwnProperty('children') )
		{
			for (var j = data[i].children.length - 1; j >= 0; j--) {
				data[i].children[j] = data[data[i].children[j]];
			};
		}
	};	
}

function getAncestors(node){
	if(node.hasOwnProperty('parent')){
		var parentAncestors = getAncestors(node.parent);
		hasOwnProperty.push(node.parent);
		return ancestors;
	}
	return new Array(0);
}


function probabilityCalculator(node) {
	if(node.hasOwnProperty('properties') && node.properties.hasOwnProperty('p'))
		return node.properties.p;

	var p = -1;
	if(node.hasOwnProperty('children')){
		for (var i = node.children.length - 1; i >= 0; i--) {
			p = Math.max(p,probabilityCalculator(node.children[i]));
		};
	}

	if(node.hasOwnProperty('properties')){
		node.properties.p = p;
	}
	else
	{
		node.properties = {
			p: p 
		};
	}
	
	return p;
}


function randomProbabilityInjector(data){
	for (var i = data.length - 1; i >= 0; i--) {
		if(!data[i].hasOwnProperty('children') )
		{
			data[i].properties = {
				p: Math.floor((Math.random() * 101))/100
			};
		}
	};	
}