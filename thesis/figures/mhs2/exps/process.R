library(ggplot2)
library(plyr)

source("common.R")

read.data = function(p){
    res = NULL
    for(d in dir(p)) {

        path = sprintf("%s/%s/stats.D.csv", p, d)
        if(file.exists(path)){
            tmp = read.csv(path)
            tmp$r = d
            res = rbind(res, tmp)
        }
    }
    res$r=factor(res$r)
    return (res)
}


cleanup.names = function(data) {
    data$generator = revalue(data$generator,
        c("mhs_baseline"="Baseline",
          "mhs_heuristic"="Heuristic",
          "mhs_pruned_tree"="Opt 1",
          "mhs_pruned_ranking"="Opts 1-2",
          "mhs_reference"="Opts 1-3"))
                                        #    data = data[which(data$generator != "Reference"),]
    return(data)
}

cleanup.names2 = function(data) {
    # data=data[which(data$generator %in% c("Heuristic", "Opts 1-3")),]
    data$generator = revalue(data$generator,
                             c("Heuristic"="Heuristic (Staccato)",
                               "Opts 1-3"="Opts 1-3 (MHS2)"))
                                        #    data = data[which(data$generator != "Reference"),]

    return(data)
}

optim.default.plot = function(...) {
    p = baseplot(...)

#    p = p + scale_color_grey(name = "MHS Algorithm")

    p = p + my.scale_color_discrete(name = "")#"MHS Algorithm")
    p = p + my.scale_shape_discrete(name = "")#"MHS Algorithm")

#    p = p + scale_x_discrete(labels=my.percent)

    p = p + geom_point()
    p = p + my.geom_line()

    return (p)
}



###### OPTIM SMALL #######
process.optim.small = function (all, avg) {
    data = read.data("all_vs_all/small/")
    data$MN = 10
    for (i in 3:8) {
        size = i * 5
        data_tmp = read.data(sprintf("optim_vs_optim/%d/", size))
        data_tmp$MN = size
        data = rbind(data, data_tmp)
    }
    data = cleanup.names(data)
    for(n in c("r", "iteration", "duration", "avg_card", "perc_minimal", "size"))
        data[,which(colnames(data)==n)] = as.numeric(as.character(data[,which(colnames(data)==n)]))
    data$r = data$r * 100

    data_avg = aggregate(cbind(duration,size, perc_minimal, avg_card)~r+MN+generator, data, mean)
    data_avg = data_avg[order(data_avg$MN, data_avg$r, data_avg$generator), ]
    rownames (data_avg) = NULL


    write.csv(data, all)
    write.csv(data_avg, avg)
}

plot.optim.small = function (data) {
    plots = list()
    data$r = data$r / 100
    data$duration = data$duration / 1e6
    data = cleanup.names2(data)
    data_tmp = data[which(data$MN %% 10 == 0),]
    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=duration,
            color=generator,
            shape=generator,
            group=generator))
    p = p + facet_grid(MN~., labeller = MN.labeller)
    p = p + my.geom_line()
    plots = append(plots, list(p + labs(x = "R", y = "Run Time")))

    p = p + scale_y_log10_custom()
    plots = append(plots, list(p + labs(x = "R", y = "Run Time (log)")))

    sizes = levels(factor(data$MN))



    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=size/(duration),
            color=generator,
            shape=generator,
            group=generator))
    p = p + facet_grid(MN~., labeller = MN.labeller)
    p = p + my.geom_line()
    p = p + scale_y_log10_custom(limits=c(0.6e-2,1e6), breaks=c(1e-2, 1e0, 1e2, 1e4, 1e6))

    plots = append(plots, list(p + labs(x = "R", y = "Throughput (log)")))

### Efficiency ###
    data_tmp$eff = 1
    for(i in 1:nrow(data_tmp)) {
        block = which((data_tmp$MN[i] == data_tmp$MN) & (data_tmp$r[i] == data_tmp$r))
        data_tmp$eff[i] = max(data_tmp$duration[block]) / data_tmp$duration[i]
    }

    avgs = aggregate(eff~generator+MN, data_tmp, mean)
    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=eff,
            color=generator,
            shape=generator,
            group=generator))
    p = p + facet_grid(MN~., labeller = MN.labeller)
    p = p + my.geom_line()
    p = p + scale_y_log10_custom()
#    p = p + geom_hline(yintercept=avgs$eff[which(avgs$generator=="MHS2" & avgs$MN==10)],color="gray", size=0.25, linetype="dashed")

    plots = append(plots, list(p + labs(x = "R", y = "Efficiency (log)")))



    return (plots)

    for (i in sizes) {
        data_tmp = data[which(data$MN==i),]
        p = optim.default.plot(
            data=data_tmp,
            aes(x=as.factor(r), y=duration, color=generator, shape=generator, group=generator))
        p = p + ggtitle(sprintf("M = N = %s", i))

        plots = append(plots, list(p + labs(x = "R", y = "Run Time (secs)")))

        p = p + scale_y_log10_custom()
        plots = append(plots, list(p + labs(x = "R", y = "Run Time (log)")))
    }





    return (plots)
}


plot.optim.small2 = function (data) {
    plots = list()
    data$r = data$r / 100
    data$duration = data$duration / 1e6

    data_tmp = data[which(data$MN %% 10 == 0),]
#    data_tmp = aggregate(cbind(avg_card,size)~r+MN, data_tmp, mean)
    data_tmp = data_tmp[which(data_tmp$generator == "Opts 1-3"),]
    data_tmp$MN = factor(data_tmp$MN)
    data_tmp$bits = data_tmp$avg_card * (data_tmp$size+1) * 32 / 1024 / 1024
    #levels(data_tmp$MN) = paste("M = N =", levels(data_tmp$MN))
    print(data_tmp)

    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=size,
            color=MN,
            shape=MN,
            group=MN))
    p = p + my.scale_shape_discrete(name = "M = N =")
    p = p + my.scale_color_discrete(name = "M = N =")
    p = p + my.geom_line()
    p = p + scale_y_log10_custom(limits=c(0.2,1e6))

    plots = append(plots, list(p + labs(x = "R", y = "|D| (log)")))

    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=avg_card,
            color=MN,
            shape=MN,
            group=MN))
    p = p + my.scale_shape_discrete(name = "M = N =")
    p = p + my.scale_color_discrete(name = "M = N =")
    p = p + my.geom_line()
    p = p + ylim(0,20)


    plots = append(plots, list(p + labs(x = "R", y = "MHS Cardinality")))


    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=bits,
            color=MN,
            shape=MN,
            group=MN))
    p = p + scale_y_log10_custom()
    p = p + my.scale_shape_discrete(name = "M = N =")
    p = p + my.scale_color_discrete(name = "M = N =")
    p = p + my.geom_line()


    plots = append(plots, list(p + labs(x = "R", y = "Transfered Data (log Mb)")))


    p = optim.default.plot(
        data=data_tmp,
        aes(x=as.factor(r),
            y=duration,
            color=MN,
            shape=MN,
            group=MN))
    p = p + scale_y_log10_custom()
    p = p + my.scale_shape_discrete(name = "M = N =")
    p = p + my.scale_color_discrete(name = "M = N =")
    p = p + my.geom_line()


    plots = append(plots, list(p + labs(x = "R", y = "Run Time (log sec)")))


    return (plots)
}







####### OPTIM LARGE ######
process.optim.large = function (all, avg) {
    data = read.data("all_vs_all/large/")
    data = cleanup.names(data)
    for(n in c("r","iteration", "duration", "avg_card", "perc_minimal", "size"))
        data[,which(colnames(data)==n)] = as.numeric(as.character(data[,which(colnames(data)==n)]))
    data$r = data$r * 100
    data$duration = data$duration / 1e6


    data_avg = aggregate(cbind(duration,size, perc_minimal, avg_card)~generator+r, data, mean)
    data_avg = data_avg[order(data_avg$r, data_avg$generator), ]

    write.csv(data, all)
    write.csv(data_avg, avg)
}


xplot.optim.large = function (data) {

    data$throughput = data$perc_minimal * data$size / data$duration
    data$eff = 1
    for(r in unique(data$r)) {
        block = which(data$r == r)

        data[block,]$eff = data[block,]$throughput / min(data[block,]$throughput)
    }


    avgs = aggregate(eff~generator, data, mean)


    p = optim.default.plot(
        data=data,
        aes(x=as.factor(r),
            y=eff,
            color=generator,
            shape=generator,
            group=generator))
    p = p + my.geom_line()
#    p = p + scale_y_log10_custom(limits=c(1e-3, 1e4))
    p = p + labs(x = "R", y = "Throughput (log)")
#    p = p + geom_hline(yintercept=avgs$eff[which(avgs$generator=="MHS2")],color="gray", size=0.25, linetype="dashed")



    return (p)
}


plot.optim.large = function (data) {
    plots = list()
    data$r = data$r / 100
    data$duration = data$duration / 1e6
    data = cleanup.names2(data)

    p = optim.default.plot(data=data, aes(x=as.factor(r), y=size, color=generator, shape=generator, group=generator))

    plots = append(plots, list(p +
        labs(x = "R", y = "# HSs")))

    plots = append(plots, list(p +
        scale_y_log10_custom(limits=c(0.8e1, 1e7), breaks=c(1e1, 1e3, 1e5, 1e7)) +
        labs(x = "R", y = "# HSs (log)")))



    p = optim.default.plot(data=data, aes(x=as.factor(r), y=avg_card, color=generator, shape=generator, group=generator))

    plots = append(plots, list(p +
        labs(x = "R", y = "Avg. Card.")))

    plots = append(plots, list(p +
        scale_y_log10_custom(limits=c(0.8,1e3), breaks=c(1, 1e1, 1e2, 1e3)) +
        labs(x = "R", y = "Avg. Card. (log)")))


    p = optim.default.plot(data=data, aes(x=as.factor(r), y=perc_minimal, color=generator, shape=generator, group=generator))
    p = p + scale_y_continuous(limits=c(-0.01,1),labels=my.percent)

    plots = append(plots, list(p +
        labs(x = "R", y = "Minimal")))

    p = optim.default.plot(
        data=data,
        aes(x=as.factor(r),
            y=perc_minimal*size/duration,
            color=generator,
            shape=generator,
            group=generator))
    p = p + my.geom_line()
    p = p + scale_y_log10_custom(limits=c(1e-4, 1e4))

    plots = append(plots, list(p + labs(x = "R", y = "Throughput (log)")))
    plots = append(plots, list(xplot.optim.large(data) + labs(x = "R", y = "Efficiency (log)")))

    return (plots)
}






#process.optim.small(get.csv.path("optim_small_all.csv"),
#                    get.csv.path("optim_small_avg.csv"))

#process.optim.large(get.csv.path("optim_large_all.csv"),
#                    get.csv.path("optim_large_avg.csv"))
