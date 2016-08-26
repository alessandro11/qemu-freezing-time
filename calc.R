#!/usr/bin/env Rscript

opt <- commandArgs(TRUE)
if( length(opt) == 0 ) {
    stop("Passe o nome do arquivo com uma coluna de valores.")
}

ConfidenceInterval <- function(conf, avg, sde, size) {
    error <- qnorm(conf) * sde / sqrt(size)
    left  <- avg - error
    right <- avg + error

    return( list("left" = left, "right" = right) )
}


vals <- read.table(opt)
un_vals <- unlist(vals)

message("Calculando valores...")
avg  <- mean(un_vals)
sde  <- sd(un_vals)
vari <- var(un_vals)
cv   <- sde / avg
mi   <- min(un_vals)
ma   <- max(un_vals)

message(paste("Media          =", round(avg, 2)))
message(paste("Variancia      =", round(vari, 2)))
message(paste("Desvio Padrao  =", round(sde, 2)))
message(paste("Coef. Variacao =", round(cv, 2)))
message(paste("Min            =", mi))
message(paste("Max            =", ma))

lr <- ConfidenceInterval(0.95, avg, sde, length(un_vals))
# message("Intervalo de confianca")
message(paste("Esquerda, Dir. =", round(lr$left, 4), ",", round(lr$right, 4)))

