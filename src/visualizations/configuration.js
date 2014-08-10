var asa;
function ConfigurationView(data, elementID, configuration, events){
	var self = this;
	this.render = function(){
		self.renderDefualtVis();
	}

	this.renderDefualtVis = function(){
		$('#'+elementID).append('<fieldset><label for="defaultVis">Select the defaut visualization:</label><select name="defaultVis" id="defaultVis"></select></fieldset>');
		$.each(visualizations, function(index, visualization) {
			$("#defaultVis").append('<option id="defaultVis_'+index+'" value="'+index+'">'+visualization.displayName+'</option>');
		});
		$('#defaultVis_'+configuration.currentConfig.defaultView).attr("selected","selected");
		$("#defaultVis").selectmenu({
			change: function( event, data ) {
				configuration.currentConfig.defaultView = data.item.value;
				configuration.saveConfig();
			}
		});
	}

	this.renderAnimationTime = function(){

	}
}