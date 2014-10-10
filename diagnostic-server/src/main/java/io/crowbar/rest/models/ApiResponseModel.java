package io.crowbar.rest.models;

import com.wordnik.swagger.annotations.ApiModel;
import com.wordnik.swagger.annotations.ApiModelProperty;

@ApiModel(value = "Api Response",
          description = "This model wraps all rest responses")
public class ApiResponseModel<T> {
    private final int code;
    private final String message;
    private final T response;

    private ApiResponseModel (int code,
                              String message,
                              T response) {
        this.code = code;
        this.message = message;
        this.response = response;
    }

    public ApiResponseModel (T response) {
        this(200, null, response);
    }

    public ApiResponseModel (int code,
                             String message) {
        this(code, message, null);
    }

    @ApiModelProperty(value = "The HTTP return code for the request",
                      required = true)
    public int getCode () {
        return code;
    }

    @ApiModelProperty(value = "The error message",
                      required = false)
    public String getMessage () {
        return message;
    }

    @ApiModelProperty(value = "The response payload",
                      required = false)
    public T getResponse () {
        return response;
    }
}