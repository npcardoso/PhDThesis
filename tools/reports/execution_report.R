library('reshape2')
library('ggplot2')

# Fix Time to start at 0 and to be in seconds
fix_job_times = function(data) {
    times = c(data$init, data$start, data$end)
    min_time = min(times)
    data$init = (data$init - min_time) / 1e6
    data$start = (data$start - min_time) / 1e6
    data$end = (data$end - min_time) / 1e6
    return(data)
}

read_job_file = function(path) {
    data = read.csv(path)
    return (fix_job_times(data))
}

melt_jobs_matrix = function(data) {
    data = data[order(data$init), ]
    data$id = 1:nrow(data)

    data = melt(data, measure.vars=c("init", "start", "end"), value.name="time")
}

determine_interval = function(data, interval=NULL) {
    times = c(data$init, data$start, data$end)
    if(is.null(interval))
        return(c(min(times), max(times)))
    return(interval)
}

slot_occupancy = function(data, interval=NULL) {
    count_active = function(data, time) {
        return (length(which(time >= data$start &
                             time <= data$end)))
    }

    interval = determine_interval(data, interval)

    times = c(data$init, data$start, data$end)
    times = times[which(times >= interval[1] & times <= interval[2])]
    times = times[order(times)]

    last_active = count_active(data, interval[1])
    tmp = data.frame(time=interval[1], active=last_active)
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

    return (tmp)
}

total_time = function(occupancy) {
    return (max(occupancy$time) - min(occupancy$time))
}

wasted_time = function(occupancy,
    n_slots=max(occupancy$active)) {

    occupancy = occupancy[order(occupancy$time), ]
    waste = 0
    for(i in 1:(nrow(occupancy) - 1)) {
        delta = occupancy$time[i + 1] - occupancy$time[i]
        waste = waste + delta * (n_slots - mean(occupancy$active[i:(i+1)]))
    }
    return (waste)
}


plot_execution_report = function(data, nbreaks=20, interval=NULL) {
    interval = determine_interval(data, interval)
                                        #Reorder data points

    data = melt_jobs_matrix(data)
    rows = which(data$time >=interval[1] & data$time <=interval[2])
    data = data[rows,]
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

    p = ggplot(tmp, aes(time, factor(id),color=type,linetype=variable))
    p = p + geom_line(size = 2)
    p = p + xlab("Time")
    p = p + ylab("Job ID")
    p = p + theme_bw()
    p = p + theme(legend.position="top")
    p = p + scale_linetype_manual(values=c(3,1))
    p = p + scale_x_continuous(breaks=x_breaks,limits=interval)
    return (p)
}


plot_slot_occupancy = function(data, interval=NULL) {
    interval = determine_interval(data, interval)
    times = c(data$init, data$start, data$end)
    min_time = min(times)

    tmp = slot_occupancy(data, interval)
    p = ggplot(tmp, aes(time, active))
    p = p + geom_line()
    p = p + xlab("Time")
    p = p + ylab("Active Jobs")
    p = p + scale_x_continuous(limits=interval)
    p = p + scale_y_continuous(limits=c(0, max(tmp$active)))
    p = p + theme_bw()

    return (p)

}


generate_execution_report = function(file, data, width=100) {
    pdf(file)

    times = c(data$init, data$start, data$end)
    max_time = max(times)

    t = min(times)
    while(t <= max_time) {
        print(plot_execution_report(data, interval=c(t, t + width)))
        print(plot_slot_occupancy(data, interval=c(t, t + width)))
        t = t + width
    }
    dev.off()
}
