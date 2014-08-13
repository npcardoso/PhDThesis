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
		parentAncestors.push(node.parent);
		return parentAncestors;
	}
	return new Array(0);
}

  function removeArray(arr, item) {
      for(var i = arr.length; i--;) {
          if(arr[i].id == item) {
              arr.splice(i, 1);
          }
      }
  }

function filterToNMostRelevant(data,N){
	var tmp = data.slice();
	tmp.sort(function(a,b){
		return b.properties.p - a.properties.p;
	});
	var maxProb = tmp[Math.min(tmp.length-1,N)].properties.p;
	probabilityFilter(data,maxProb);
}

function probabilityFilter(data,pFilter){
		for (var i = data.length - 1; i >= 0; i--) {
			if(data[i].properties.p < pFilter)
			{
				var parent = data[data[i].parent_id];
				if(parent != null && parent.hasOwnProperty('children')){
					removeArray(parent.children,data[i].id);
				}
				data.splice(i, 1);
			}
		}
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