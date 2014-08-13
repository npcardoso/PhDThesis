var tableData,dataWithNoChildren;
function Table(data, elementID, configuration, events) {
    var self = this;
    dataWithNoChildren = jQuery.grep(data, function( node, index ) {
        return (!node.hasOwnProperty('children') || node.children == null || node.children.length == 0);
    });
    var parent, gParent;
    tableData = jQuery.map(dataWithNoChildren, function( node, index ) {
        if(node != null){
            parent = data[node.parent_id];
            if(parent  != null){
                gParent = data[parent.parent_id];
                if(gParent != null){
                    return [[gParent.name,parent.name,node.name,node.properties.p,node]];
                }
            }
        }
    });

    this.render = function(){
        $('#'+elementID).html('<table cellpadding="0" cellspacing="0" border="0" class="display" id="tableData"></table>');
        table = $('#tableData').dataTable({
            "retrieve" : true,
            "data" : tableData,
            "columns" : [{
                "title" : "Class"
            }, {
                "title" : "Function"
            }, {
                "title" : "Line",
                "class" : "center"
            }, {
                "title" : "Error Score",
                "class" : "center"
            }],
            "order" : [3, "desc"],
        });

        $('#tableData tbody').on('click', 'tr', function() {
            if ($(this).hasClass('selected')) {
                $(this).removeClass('selected');
            } else {
                table.$('tr.selected').removeClass('selected');
                $(this).addClass('selected');
            }
            location.hash = "asa";
        });
    }
    
}