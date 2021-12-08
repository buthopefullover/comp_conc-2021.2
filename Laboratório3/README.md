# Laboratório 3: encontrar a quantidade de valores de um vetor de numeros reais que estão dentro de uma faixa de valores dada. 
### Para calcularmos a aceleração obtida executaremos 5 vezes cada dimensão proposta pela professora.
### Para dimensão 10^5 e número de threads 1, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.000911     |      0.003082     |
|  2  |     0.000881     |      0.001273     |
|  3  |     0.000815     |      0.001274     |
|  4  |     0.000891     |      0.001542     |
|  5  |     0.000986     |      0.001446     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.000815/0.001273 ~= 0.640219
### Para dimensão 10^5 e número de threads 2, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.000881     |      0.001756     |
|  2  |     0.000873     |      0.000792     |
|  3  |     0.000894     |      0.000836     |
|  4  |     0.000898     |      0.000863     |
|  5  |     0.000966     |      0.001068     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.000873/0.000792 ~= 1.102272
### Para dimensão 10^5 e número de threads 4, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.000851     |      0.003109     |
|  2  |     0.000881     |      0.000908     |
|  3  |     0.000841     |      0.002371     |
|  4  |     0.000858     |      0.001031     |
|  5  |     0.000842     |      0.004120     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.000841/0.000908 ~= 0.926211
### Para dimensão 10^7 e número de threads 1, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.093859     |      0.086531     |
|  2  |     0.089234     |      0.094746     |
|  3  |     0.090503     |      0.087089     |
|  4  |     0.094186     |      0.088917     |
|  5  |     0.091491     |      0.091008     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.089234/0.086531 ~= 1.031237
### Para dimensão 10^7 e número de threads 2, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.093933     |      0.046086     |
|  2  |     0.090089     |      0.048925     |
|  3  |     0.089189     |      0.047410     |
|  4  |     0.098204     |      0.047760     |
|  5  |     0.092472     |      0.051359     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.089189/0.046086 ~= 1.935273
### Para dimensão 10^7 e número de threads 4, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.094210     |      0.032452     |
|  2  |     0.130822     |      0.031577     |
|  3  |     0.090465     |      0.033256     |
|  4  |     0.095570     |      0.031498     |
|  5  |     0.090502     |      0.031654     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.090465/0.031498 ~= 2.872087
### Para dimensão 10^8 e número de threads 1, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.905327     |      0.909921     |
|  2  |     0.959303     |      0.906403     |
|  3  |     0.889429     |      0.942197     |
|  4  |     0.863871     |      0.895953     |
|  5  |     0.868758     |      0.915422     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.863871/0.895953 ~= 0.964192
### Para dimensão 10^8 e número de threads 2, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.874840     |      0.512065     |
|  2  |     0.876517     |      0.486540     |
|  3  |     0.942790     |      0.691398     |
|  4  |     0.857782     |      0.457365     |
|  5  |     0.866274     |      0.444158     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.857782/0.444158 ~= 1.931254
### Para dimensão 10^8 e número de threads 4, temos:
| Vez | Tempo Sequencial | Tempo Concorrente |
|:---:|:----------------:|:-----------------:|
|  1  |     0.877412     |      0.496935     |
|  2  |     0.944077     |      0.319028     |
|  3  |     0.869604     |      0.301736     |
|  4  |     0.868869     |      0.322452     |
|  5  |     0.869150     |      0.329425     |

### Pegando o menor tempo sequencial e concorrente para realizar o cálculo de ganho através da lei de Amdahl:
#### 0.868869/0.301736 ~= 2.879566
