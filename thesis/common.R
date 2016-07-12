library(scales)   # Need the scales package
my.geom_line = function (...) {
    return (geom_line(...,size=0.2))
}
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

my.percent = function(x) {
    return(paste(as.integer(x*100), "%", sep=""))
}
scientific_labeller <- function(x) {
    expn = floor(log10(x))
    mult = x / (10 ** expn)

    mult = round(mult,2)
    mult_str = sprintf('%.2f', mult)
    mult_str <- gsub('0*$', '', mult_str)
    mult_str <- gsub('\\.$', '', mult_str)
    ret = sprintf('%se%0.0f', mult_str, floor(expn))
    ret [which(abs(x) < 0.0001)] = "0"
    return (ret)
}

scale_y_log10_custom = function(...) {
    return (scale_y_log10(..., label = scientific_labeller))# labels = trans_format("log10", math_format(10^.x))))
}

scale_x_log10_custom = function(...) {
    return (scale_x_log10(..., labels = trans_format("log10", math_format(10^.x))))
}

get.csv.path = function(name) {
    return (paste("csvs", name, sep="/"))
}

get.pdf.path = function(name) {
    return (paste("pdfs", name, sep="/"))
}

open.pdf = function(name, ...) {
    library(extrafont)
    font_import()
    font_install("fontcm")

    return (pdf(get.pdf.path(name), ...))#, family="CM Roman", ...))
}

simpleCap <- function(x) {
    return(gsub("(^|[[:space:]])([[:alpha:]])", "\\1\\U\\2", x, perl=TRUE))
}

trim <- function (x) gsub("^\\s+|\\s+$", "", x)

baseplot = function(...) {
    p = ggplot(...)
    base_size = 20
    ## options(scipen=0)
    # p = p + theme_classic(base_size = base_size)
    p = p + theme(
                axis.line = element_line(size = 0.4, color = "black"),
                axis.line.x = element_blank(),
                axis.line.y = element_line(colour = "black", size = 0.5),
                axis.text.y = element_text(angle = 90, hjust = 0.5),
                axis.ticks = element_line(colour = "black", size = 0.5),
                legend.background = element_rect(fill = "transparent",colour = NA),
                legend.key = element_rect(fill = NA, colour = "#333333", size = 0.25),
                legend.position = "top",
                legend.title = element_text(size = 8, face = "bold", hjust = 0),
                panel.background = element_rect(fill = "transparent",colour = NA),
                panel.border = element_blank(),
                panel.grid.minor = element_blank(),
                panel.grid.major = element_line(size = 0.1, color = "#CCCCCC"),
                plot.background = element_rect(fill = "transparent",colour = NA),
                text = element_text(size = 9),
                strip.background = element_blank()
            )

    return (p)
}

total.time = function(nproc, walltime) {
    return(nproc*walltime)
}

mhs.per.sec = function(count, nproc, walltime) {
    return(count / total.time(nproc,walltime))
}

convert.to.numeric = function(data, fields) {
    for(n in fields)
        data[,which(colnames(data)==n)] = as.numeric(as.character(data[,which(colnames(data)==n)]))
    return (data)
}

MN.labeller = function(s,...) {
    s[,1] = sprintf("M = N = %d", s[,1])
    return (s)
}

R.labeller = function(s) {
    print(s)
    for (x in 1:ncol(s)){
        if(colnames(s)[x] == "r")
            s[,x] = sprintf("R = %.2f", s[,x])
        else
            s[,x] = sprintf("%s", s[,x])

    }
    return (s)
}

speedup = function(ref, data) {
    return (ref/data)
}

efficiency = function(sup, np) {
    return (sup/np)
}

improvements = function(data, field_names, stride) {
    for (f in field_names) {
        field_num = which(colnames(data) == f)
        sup = NULL
        for(i in 0:(nrow(data) / stride - 1)) {
            start = i * stride + 1
            end = (i + 1) * stride
            sup = append(sup,
                speedup(data[start,field_num],
                        data[start:end,field_num]))
        }


        f_names = c(
            paste("sup_", f, sep=""),
            paste("invsup_", f, sep=""),
            paste("eff_", f, sep=""),
            paste("inveff_", f, sep=""))

        eff = efficiency(sup, 1:stride)
        inv_sup = 1 / sup
        inv_eff = efficiency(inv_sup, 1:stride)

        res = data.frame(sup, inv_sup, eff, inv_eff)
        colnames(res) = f_names
        data = cbind(data, res)
    }
    return (data)
}
