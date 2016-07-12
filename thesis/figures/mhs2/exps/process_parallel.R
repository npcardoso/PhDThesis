library(scales)
library(reshape)
library(ggplot2)
library(plyr)
library(xtable)
source("common.R")



read.data = function(p){
    res = NULL
    for(d in dir(p)) {

        path = sprintf("%s/%s/stats.D.csv", p, d)
        if(file.exists(path)){
            tmp = read.csv(path,stringsAsFactors=FALSE)
            tmp = cast(tmp)
            tmp$r = d
            res = rbind(res, tmp)
        }
    }
    colnames(res) = trim(colnames(res))

    for(n in c("iteration", "duration", "avg_card", "perc_minimal", "size"))
        res[,which(colnames(res)==n)] = as.numeric(as.character(res[,which(colnames(res)==n)]))
    res$generator=factor(res$generator)
    res$r=factor(res$r)
    res$duration = res$duration / 1e6
    return (res)
}

parse.names = function(data, field_names, separator) {
    ret = NULL
    for (d in data) {
        parsed = trim(unlist(strsplit(d, separator)))
        ret = rbind(ret, parsed)
    }
    colnames(ret) = field_names[1:ncol(ret)]
    rownames(ret) = NULL
    return (ret)
}




parallel.default.plot = function(...) {
    p = baseplot(...)


    return (p)
}


parallel.small.default.plot = function(...) {
    p = parallel.default.plot(...)
#    p = p + scale_color_grey(name = "Setup", start=0, end=0.8)
    p = p + my.scale_color_discrete(name = "Setup")
    p = p + my.scale_shape_discrete(name = "Setup")
 #   p = p + scale_x_discrete()

    p = p + geom_point()
    p = p + my.geom_line()

    p = p + facet_grid(.~r, labeller = R.labeller)

    return (p)
}


parallel.large.default.plot = function(...) {
    p = parallel.default.plot(...)
    #p = p + scale_color_grey(name = "Time Cutoff", start=0, end=0.8)

    p = p + my.scale_color_discrete(name = "Cutoff Time")
    p = p + my.scale_shape_discrete(name = "Cutoff Time")
    p = p + scale_y_continuous(label = scientific_labeller)
                                        #    p = p + scale_x_discrete()

    p = p + geom_point()
    p = p + my.geom_line()

    p = p + facet_grid(par_strategy~r, labeller = R.labeller)

    return (p)
}





process.parallel = function (path, all, avg) {
    data = read.data(path)
    data$generator = trim(data$generator)
    data = convert.to.numeric(data, c("r", "iteration", "duration", "avg_card", "perc_minimal", "size"))
    data$r = data$r * 100
#    data$duration = data$duration / 1e6
    data$size_minimal = data$perc_minimal * data$size

    data_avg = aggregate(.~generator+r, data, mean)
    data_avg = cbind(parse.names(data_avg$generator, c("name", "par_strategy", "nproc", "time"), "_"), data_avg)
    data_avg = convert.to.numeric(data_avg, c("nproc", "time"))
    levels(data_avg$par_strategy) = simpleCap(levels(data_avg$par_strategy))



    data_avg$mhspersec = mhs.per.sec(data_avg$size*data_avg$perc_minimal, data_avg$nproc, data_avg$duration)
    data_avg$totaltime = total.time(data_avg$nproc, data_avg$duration)



    write.csv(data, all)
    write.csv(data_avg, avg)
}

plot.parallel.small = function(data_all, data_avg) {
    plots = list()
    data_all$r = data_all$r / 100
    data_avg$r = data_avg$r / 100
#    data_avg$duration = data_avg$duration / 1e6
#    data_all$duration = data_all$duration / 1e6

    data_all = cbind(parse.names(data_all$generator, c("name", "par_strategy", "nproc", "time"), "_"), data_all)
    data_all = convert.to.numeric(data_all, c("nproc", "time"))
    levels(data_all$par_strategy) = simpleCap(levels(data_all$par_strategy))

    data_tmp = improvements(data_avg, c("duration"),8)

    data_tmp$nproc = as.factor(data_tmp$nproc)
    p = parallel.small.default.plot(
        data=data_tmp,
        aes(x=nproc,
            y=sup_duration,
            color=par_strategy,
            shape=par_strategy,
            group=par_strategy))

    p = p + labs(x="# CPUs", y="Speedup")
    p = p + ylim(0,8)

    plots = append(plots, list(p))


    p = parallel.small.default.plot(
        data=data_tmp,
        aes(x=nproc,
            y=eff_duration,
            color=par_strategy,
            shape=par_strategy,
            group=par_strategy))
    p = p + scale_y_continuous(limits=c(0.26, 1.4), breaks=c(0.33, 0.66, 1, 1.33), labels=percent)
    p = p + labs(x="# CPUs", y="Efficiency")
    plots = append(plots, list(p))

    p = parallel.small.default.plot(
        data=data_tmp,
        aes(x=nproc,
            y=mhspersec,
            color=par_strategy,
            shape=par_strategy,
            group=par_strategy))

    p = p + scale_y_continuous(limits=c(0,4e4), label=scientific_labeller)
    p = p + labs(x="# CPUs", y="Throughput")
    plots = append(plots, list(p))

    p = parallel.default.plot(
        data=data_all,
        aes(x=as.factor(nproc),
            y=duration,
            color=par_strategy,
            shape=par_strategy,
            group=par_strategy))
    p = p + stat_summary(fun.y=mean, geom="point",shape=16)
    p = p + stat_summary(fun.y=mean, geom="line",size=0.2)
    p = p + geom_jitter(alpha=0.3,shape=16,size=1,position = position_jitter(h=0,w=0.1))
    p = p + facet_grid(.~r, labeller = R.labeller)
    p = p + scale_y_log10_custom(limits=c(0.5e-3,1.2e3),breaks=c(1e-3,1e-1,1e1,1e3))
    p = p + my.scale_color_discrete(name = "Setup")
    p = p + my.scale_shape_discrete(name = "Setup")


    p = p + labs(x="# CPUs", y="Run Time (log)")
    plots = append(plots, list(p))

    return (plots)
}

















plot.parallel.large = function(data) {
    plots = list()
    data$r = data$r / 100

    data = convert.to.numeric(data, c("time", "nproc"))
#    data$duration = data$duration / 1e6
    data$time = data$time/ 1e6
    data$lbltime = factor(data$time)
    levels(data$lbltime) = paste(levels(data$lbltime), "sec")

    data$nproc = as.factor(data$nproc)


    p = parallel.large.default.plot(
        data=data,
        aes(x=nproc,
            y=perc_minimal,
            color=lbltime,
            shape=lbltime,
            group=lbltime))
    p = p + scale_y_continuous(label=percent,
                               limits=c(0.65, 1))
    p = p + labs(x="# CPUs", y="Minimal")
    plots = append(plots, list(p))

    p = parallel.large.default.plot(
        data=data,
        aes(x=nproc,
            y=size,
            color=lbltime,
            shape=lbltime,
            group=lbltime))
    p = p + scale_y_continuous(label=scientific_labeller)
    p = p + labs(x="# CPUs", y="# HS")
    plots = append(plots, list(p))

    p = parallel.large.default.plot(
        data=data,
        aes(x=nproc,
            y=size * perc_minimal,
            color=lbltime,
            shape=lbltime,
            group=lbltime))
    p = p + scale_y_continuous(label=scientific_labeller, breaks=c(0,2e5,4e5,6e5))
#    p = p + scale_y_log10_custom()
    p = p + labs(x="# CPUs", y="# MHS")
    plots = append(plots, list(p))


############# MHS Speedup/Efficiency ##########
    tmp = data[order(data$r, data$par_strategy, data$time), ]
    tmp = improvements(tmp, c("avg_card", "perc_minimal", "size", "size_minimal"), 8)
    p = parallel.large.default.plot(
        data=tmp,
        aes(x=nproc,
            y=invsup_size_minimal,
            color=lbltime,
            shape=lbltime,
            group=lbltime))
    p = p + labs(x="# CPUs", y="MHS Speedup")
    plots = append(plots, list(p))

    p = parallel.large.default.plot(
        data=tmp,
        aes(x=nproc,
            y=inveff_size_minimal,
            color=lbltime,
            shape=lbltime,
            group=lbltime))
    p = p + labs(x="# CPUs", y="MHS Efficiency")
    p = p + scale_y_continuous(label=percent, breaks=c(1,1.5))
    plots = append(plots, list(p))

############# MHS per second ##########
    p = parallel.large.default.plot(
        data=data,
        aes(x=nproc,
            y=mhspersec,
            color=lbltime,
            shape=lbltime,
            group=lbltime))
    p = p + scale_y_continuous(label=scientific_labeller,
                               limits=c(0, 4e4))
    p = p + labs(x="# CPUs", y="Throughput")
    plots = append(plots, list(p))

    return (plots)

}









parallel.large.scatter.default.plot = function(...) {
    p = parallel.default.plot(...)
    p = p + scale_x_continuous()
    p = p + scale_y_continuous(labels=scientific_labeller)

    p = p + scale_size(guide="none")
                                        #    p = p + scale_color_grey(name = "CPUs", start=0.8, end=0.2)
    p = p + scale_shape_manual(guide="none", values=as.character(1:8), name = "CPUs")
    p = p + my.scale_color_discrete(name = "Cutoff Time")

    p = p + geom_point(size=2)

    return (p)
}


plot.parallel.large.scatter = function(data) {
##############Scatter plots ################
    plots = list()
    data$r = data$r / 100

    data= aggregate(.~time+nproc, data, mean)

    data = convert.to.numeric(data, "time")
    print(as.numeric(data$time) / 1e6)
    print(head(data))
    data$lbltime = factor(data$time /1e6)
    levels(data$lbltime) = paste(levels(data$lbltime), "sec")


    p = parallel.large.scatter.default.plot(
        data=data,
        aes(x=totaltime,
            y=size*perc_minimal,
            color=lbltime,
            shape=factor(nproc)))
    p = p + scale_y_continuous(labels=scientific_labeller,limits=c(0,5e5))
    plots = append(plots, list(p + labs(x="Total CPU Time", y="# MHSs")))

    p = p + scale_y_log10_custom(limits=c(10000, 1000000))
    p = p + scale_x_log10_custom()
    plots = append(plots, list(p + labs(x="Total CPU Time (log)", y="# MHSs (log)")))


    p = parallel.large.scatter.default.plot(
        data=data,
        aes(x=totaltime,
            y=mhspersec,
            color=lbltime,
            shape=as.factor(nproc)))
    p = p + scale_y_continuous(labels=scientific_labeller,limits=c(0,2.5e4))
    plots = append(plots, list(p + labs(x="Total CPU Time", y="Throughput")))

    p = p + scale_x_log10_custom()
    p = p + scale_y_log10_custom(limits=c(1000, 100000))
    plots = append(plots, list(p + labs(x="Total CPU Time (log)", y="Throughput (log)")))

    return (plots)
}


#process.parallel("old/xx/small/",
#                 get.csv.path("parallel_small_all.csv"),
#                 get.csv.path("parallel_small_avg.csv"))

#process.parallel("old/xx/large/",
#                 get.csv.path("parallel_large_all.csv"),
#                 get.csv.path("parallel_large_avg.csv"))
