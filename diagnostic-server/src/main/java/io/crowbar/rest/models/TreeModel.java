package io.crowbar.rest.models;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.diagnostic.spectrum.Tree;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;
import java.util.List;

@ApiModel(value = "Tree",
          description = "Tree resource representation")
public final class TreeModel {
    private final Tree original;
    public TreeModel (Tree original) {
        this.original = original;
    }

    @ApiModelProperty(value = "Tree's nodes", required = true)
    public List<NodeModel> getNodes () {
        ListWrapper.WrapperFactory<Node, NodeModel> factory =
            new ListWrapper.WrapperFactory<Node, NodeModel> () {
            @Override
            public NodeModel create (Node original) {
                return new NodeModel(original);
            }
        };

        return new ListWrapper<Node, NodeModel> (original.getNodes(),
                                                 factory);
    }
}