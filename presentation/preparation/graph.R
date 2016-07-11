library(ggplot2)

exoticPalette = c("#008080", "#ff8000", "#4384a1", "#ff4c4c", "#99cc00", "#fe211b", "#b4a55a")
    cbPalette = c("#333333", "#E69F00", "#56B4E9", "#009E73", "#0072B2", "#D55E00", "#F0E442", "#CC79A7")

defaultPalette = exoticPalette

my.scale_color_discrete = function (...) {
    return (scale_color_manual(...,values=defaultPalette))
}

my.scale_fill_discrete = function (...) {
    return (scale_fill_manual(...,values=defaultPalette))
}
my.scale_shape_discrete = function (...) {
    return (scale_shape_manual(...,values=c(2,1,4,3,5,8,7,6)))
}


read.slide.chrono.file = function(filename) {
    data = read.csv(filename, stringsAsFactors=F, header=F)
    colnames(data) = c('slide', 'time')
    data$time = sapply(data$time, FUN=as.difftime, format="%X",units="secs")

    data$critical = data$time > mean(data$time)
    data$critical = data$time > mean(data$time) +  sd(data$time)
    data$perc = (cumsum(data$time)/sum(data$time)) * (max(data$time) - min(data$time)) + min(data$time)

    return (data)
}

plot.time.per.slide = function(filename) {
    data = read.slide.chrono.file(filename)

    p = ggplot(data, aes(slide, time))

    p = p + geom_histogram(aes(fill=critical),stat="identity")
    p = p + scale_x_discrete(breaks=data$slide)
    p = p + scale_y_discrete(breaks=seq(0,max(data$time),10))
    p = p + my.scale_fill_discrete()

    p = p + geom_line(aes(y=perc))
    p = p + geom_point(aes(y=perc))

    p = p + geom_hline(yintercept = mean(data$time),color="red")

    return (p)
}


print(plot.time.per.slide("times.19.csv"))
