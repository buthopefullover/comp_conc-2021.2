/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

class V {
  private int var;

  public V(int var){
    this.var = var;
  }

  void setVar (int value){
    this.var = value;
  }

  int getVar(){
    return this.var;
  }
}

// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit, escr;  
  
  // Construtor
  LE() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id) {
    try { 
      while (this.escr > 0) {
      //if (this.escr > 0) {
         System.out.println ("LeitorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.leit++;  //registra que ha mais um leitor lendo
      System.out.println ("LeitorLendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println ("LeitorSaindo("+id+")");
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id) {
    try { 
      while ((this.leit > 0) || (this.escr > 0)) {
      //if ((this.leit > 0) || (this.escr > 0)) {
         System.out.println ("EscritorBloqueado("+id+")");
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println ("EscritorEscrevendo("+id+")");
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     System.out.println ("EscritorSaindo("+id+")");
  }
}


// Leitora
class Leitora extends Thread {
  private int id; //identificador da thread
  private int delay;
  private V v; //atraso bobo
  LE monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Leitora (int id, int delay, V v, LE m) {
    this.id = id;
    this.delay = delay;
    this.v = v;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try{
      this.monitor.EntraLeitor(this.id);
      if (v.getVar()%2 == 0){
        System.out.println("Variável é par");
      }
      else{
        System.out.println("Variável é ímpar");
      }
      this.monitor.SaiLeitor(this.id);
      sleep(this.delay); // atraso bobo
    } catch (InterruptedException e) { return; }
  }
}


// Escritora
class Escritora extends Thread {
  private int id; //identificador da thread
  private int delay;
  private V v;
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritora (int id, int delay, V v, LE m) {
    this.id = id;
    this.delay = delay;
    this.v = v;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    try{
      this.monitor.EntraEscritor(this.id); 
      this.v.setVar(this.id);
      this.monitor.SaiEscritor(this.id); 
      sleep(this.delay); // atraso bobo
    } catch (InterruptedException e) { return; }
  }
}

class LeitoraEscritora extends Thread {
  private int id; //identificador da thread
  private int delay;
  private V v;
  
  LE monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  LeitoraEscritora (int id, int delay, V v, LE m) {
    this.id = id;
    this.v = v;
    this.monitor = m;
    this.delay = delay;
  }

  // Método executado pela thread
  public void run () {
    double bobo = 777777777.7;
    try {
      this.monitor.EntraLeitor(this.id); 
      System.out.println("Valor da variável: " + this.v.getVar());
      this.monitor.SaiLeitor(this.id); 

      for (int i=0; i<100000000; i++) {bobo = bobo/2;} // loop bobo

      this.monitor.EntraEscritor(this.id); 
      this.v.setVar(this.v.getVar()+1);
      this.monitor.SaiEscritor(this.id); 
      
      sleep(this.delay); // atraso bobo
    } catch (InterruptedException e) {  return;  } 
  }
}


// Classe principal
public class Init {
  static final int L = 4;
  static final int E = 3;
  static final int LE = 4;
  

  public static void main (String[] args) {
    LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
    Leitora[] l = new Leitora[L];       // Threads leitores
    Escritora[] e = new Escritora[E];   // Threads escritores
    LeitoraEscritora[] le = new LeitoraEscritora[LE]; // Threads lei
    V v = new V(0); // Classe para passar a variável compartilhada
    
    for (int i=0; i<L; i++) {
       l[i] = new Leitora(i, (i+1)*500, v, monitor);
       l[i].start(); 
    }
    for (int i=0; i<E; i++) {
       e[i] = new Escritora(i, (i+1)*500, v, monitor);
       e[i].start(); 
    }
    for (int i=0; i<LE; i++) {
      le[i] = new LeitoraEscritora(i, (i+1)*500, v, monitor);
      le[i].start(); 
    }
  }
}
