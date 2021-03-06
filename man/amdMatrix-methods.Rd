% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/amdMatrix.R
\docType{methods}
\name{amdMatrix}
\alias{amdMatrix}
\alias{amdMatrix,gpuMatrix-method}
\alias{amdMatrix,matrix}
\alias{amdMatrix,matrix-method}
\alias{amdMatrix,missing}
\alias{amdMatrix,missing-method}
\alias{amdMatrix,numeric}
\alias{amdMatrix,numeric-method}
\title{Construct an amdMatrix}
\usage{
amdMatrix(data = NA, nrow = NA, ncol = NA, type = NULL, ...)

\S4method{amdMatrix}{matrix}(data, type = NULL, ctx_id = NULL)

\S4method{amdMatrix}{missing}(data, nrow = NA, ncol = NA, type = NULL,
  ctx_id = NULL)

\S4method{amdMatrix}{gpuMatrix}(data, type = NULL)

\S4method{amdMatrix}{numeric}(data, nrow, ncol, type = NULL, ctx_id = NULL)
}
\arguments{
\item{data}{An object that is or can be converted to a 
\code{matrix} object}

\item{nrow}{An integer specifying the number of rows}

\item{ncol}{An integer specifying the number of columns}

\item{type}{A character string specifying the type of amdMatrix.  Default
is NULL where type is inherited from the source data type.}

\item{...}{Additional method to pass to amdMatrix methods}
}
\value{
A amdMatrix object
}
\description{
Construct a amdMatrix of a class that inherits
from \code{amdMatrix}
}
\author{
Charles Determan Jr.
}

