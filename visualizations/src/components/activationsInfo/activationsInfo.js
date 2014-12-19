function ActivationsInfo(configuration) {
	var self = this;
	self.renderTableLines = function(node){
		if(node.hasOwnProperty('freq')){
			return '<tr><td>Non-activations in passing tests:</td><td>'+node.freq[0]+'</td></tr>'
			+'<tr><td>Non-activations in failing tests:</td><td>'+node.freq[1]+'</td></tr>'
			+'<tr><td>Activations in passing tests:</td><td>'+node.freq[2]+'</td></tr>'
			+'<tr><td>Activitaions in failing tests:</td><td>'+node.freq[3]+'</td></tr>';

		}
		else
		{
			return "";
		}
	}
	self.tooltipContent = function(node){
		return '<table>'
		+renderTableLines(node)
		+'</table>';
	}

	self.renderTooltip = function(sel){
		sel.tooltipster({arrow: false, positionTracker: true, contentAsHTML: true});
	}
	return self;
}