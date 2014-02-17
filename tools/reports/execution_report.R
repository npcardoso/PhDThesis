library('reshape2')
library('ggplot2')

plot_execution_report = function(data, nbreaks=20) {

                                        #Reorder data points
    data = data[order(data$init), ]
    data$id = 1:nrow(data)

    data = melt(data, measure.vars=c("init", "start", "end"))
                                        # Fix Time to start at 0 and to be in seconds
    data$value = (data$value - min(data$value)) / 1e6
                                        # Get init and end points

    tmp = data[-which(data$variable == "start"),]
    tmp$variable = factor(tmp$variable)
                                        # Relabel
    levels(tmp$variable) = c("inactive", "active")
                                        # Get start points
    tmp2 = data[which(data$variable == "start"),]
    tmp2$variable = factor(tmp2$variable)
                                        # Add start points as active/inactive points
    levels(tmp2$variable) = "active"
    tmp = rbind(tmp, tmp2)
    levels(tmp2$variable) = "inactive"
    tmp = rbind(tmp, tmp2)



    max_x = max(tmp$value)
    x_breaks = round((0:(nbreaks+1))/nbreaks * max_x, 1)

    p = ggplot(tmp, aes(value, factor(id),color=type,linetype=variable))
    p = p + geom_line(size = 2)
    p = p + xlab("Time")
    p = p + ylab("Job ID")
    p = p + theme_bw()
    p = p + scale_linetype_manual(values=c(3,1))
    p = p + scale_x_continuous(breaks=x_breaks)
    return (p)
}


plot_slot_occuppancy = function(data, npoints=1000) {
    count_active = function(data, time) {
        count = 0
        for (i in 1:nrow(data)) {
            if(time >= data$start[i] & time <= data$end[i])
                count = count + 1
        }
        return (count)
    }

    min_time = min(data$init, data$start, data$end)
    max_time = max(data$init, data$start, data$end)

    times = min_time + ((0:(npoints + 1)) / npoints * (max_time - min_time))

    tmp = NULL
    for(t in times) {
        tmp = rbind(tmp,
            data.frame(time=t,
                       active=count_active(data, t)))
    }

    tmp$time = (tmp$time - min_time) / 1e6
    print (tmp)
    p = ggplot(tmp, aes(time, active))
    p = p + geom_line()
    p = p + xlab("Time")
    p = p + ylab("Active Jobs")
    p = p + theme_bw()
    return (p)

}
