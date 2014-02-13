plot_execution_report = function(data) {
    data = melt(data, measure.vars=c("init", "start", "end"))
                                        # Fix Time to start at 0 and to be in seconds
    data$value = (data$value - min(data$value)) / 1e6
                                        # Get init and end points
    tmp = xx[-which(xx$variable == "start"),]
    tmp$variable = factor(tmp$variable)
                                        # Relabel
    levels(tmp$variable) = c("inactive", "active")
                                        # Get start points
    tmp2 = xx[which(xx$variable == "start"),]
    tmp2$variable = factor(tmp2$variable)
                                        # Add start points as active/inactive points
    levels(tmp2$variable) = "active"
    tmp = rbind(tmp, tmp2)
    levels(tmp2$variable) = "inactive"
    tmp = rbind(tmp, tmp2)

    p = ggplot(tmp, aes(value, factor(id),color=type,linetype=variable))
    p = p + geom_line(size = 2)
    p = p + xlab("Time")
    p = p + ylab("Job ID")
    p = p + theme_bw()
    p = p + scale_linetype_manual(values=c(3,1))
    p = p + scale_x_continuous(breaks=(round((0:21)/20 * max(tmp$value), 1)))
    return (p)
}
