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