library('reshape2')
library('ggplot2')

plot_execution_report = function(data, nbreaks=20, interval=NULL) {

                                        #Reorder data points
    data = data[order(data$init), ]
    data$id = 1:nrow(data)

    data = melt(data, measure.vars=c("init", "start", "end"))
                                        # Fix Time to start at 0 and to be in seconds
    data$value = (data$value - min(data$value)) / 1e6

    if(is.null(interval))
        interval = c(min(data$value), max(data$value))
    else {
        rows = which(data$value >=interval[1] & data$value <=interval[2])
        data = data[rows,]
    }


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


    breaks = (0:nbreaks)/nbreaks
    x_breaks = round(interval[1] + breaks * (interval[2] - interval[1]), 1)

    p = ggplot(tmp, aes(value, factor(id),color=type,linetype=variable))
    p = p + geom_line(size = 2)
    p = p + xlab("Time")
    p = p + ylab("Job ID")
    p = p + theme_bw()
    p = p + theme(legend.position="top")
    p = p + scale_linetype_manual(values=c(3,1))
    p = p + scale_x_continuous(breaks=x_breaks,limits=interval)
    return (p)
}


plot_slot_occuppancy = function(data, interval=NULL) {
    count_active = function(data, time) {
        return (length(which(time >= data$start &
                             time <= data$end)))
    }

    times = c(data$init, data$start, data$end)
    min_time = min(times)
    if(is.null(interval)) {
        interval = c(min_time, max(times))
    }
    else {
        interval = interval * 1e6 + min_time
    }

    times = times[which(times >= interval[1] & times <= interval[2])]
    times = times[order(times)]

    tmp = data.frame(time=interval[1], active=0)
    last_active = 0
    for(t in times) {
        active = count_active(data, t)
        if(active != last_active) {
            tmp = rbind(tmp,
                data.frame(time=c(t,t),
                           active=c(last_active,active)))
        }
        last_active = active
    }
    tmp = rbind(tmp,
        data.frame(time=interval[2],
                   active=last_active))

    tmp$time = (tmp$time - min_time) / 1e6
    p = ggplot(tmp, aes(time, active))
    p = p + geom_line()
    p = p + xlab("Time")
    p = p + ylab("Active Jobs")
    p = p + scale_x_continuous(limits=(interval - min_time) / 1e6)
    p = p + theme_bw()

    return (p)

}


generate_execution_report = function(file, data, width=100) {
    pdf(file)

    times = c(data$init, data$start, data$end)
    max_time = (max(times) - min(times)) / 1e6

    t = 0
    while(t <= max_time) {
        print(plot_execution_report(data, interval=c(t, t + width)))
        print(plot_slot_occuppancy(data, interval=c(t, t + width)))
        t = t + width
    }
    dev.off()
}
