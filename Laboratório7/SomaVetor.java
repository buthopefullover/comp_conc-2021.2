import java.util.Random;

// classe da estrutura de dados (recurso) compartilhado entre as threads
class V {
    private int[] array;
    private int sum;

    public V(int size) {
        this.array = new int[size];

        Random randNum = new Random();
        for (int i = 0; i < size; i++) {
            this.array[i] = randNum.nextInt(10);

         }
    }

    public int size(){
        return this.array.length;
    }

    public void print() {
        for(int i = 0; i < this.array.length; i++) {
            if(i == 0) System.out.print("[" + this.array[i]);
            else System.out.print(", " + this.array[i]);
        }
        System.out.println("]");
    }

    public void setArray (int index, int value){
        this.array[index] = value;
    }

    public int getArray (int index){
        return this.array[index];
    }

    public void setSum (int value){
        this.sum += value;
    }

    public int getSum (){
        return this.sum;
    }

}

// classe que estende Thread e implementa a tarefa de cada thread do programa 
class T extends Thread{
    private int id, n_threads;
    private V array;

    public T(int id, int n_threads, V array){
        this.id = id;
        this.n_threads = n_threads;
        this.array = array;
    }

    public void run() {
        // realiza a soma intercalada de acordo com o id da thread
        for(int i = id; i < array.size(); i += n_threads) {
            this.array.setSum(this.array.getArray(i));
        }
    }
}

public class SomaVetor {
    public static void main (String[] args){
        int sum = 0; // variável para armazenar o resultado da soma sem thread

        // checa se todos os argumentos necessários foram passados
        if(args.length < 2) {
            System.out.println("Digite: java SomaVetor <tamanho do vetor> <número de threads>");
            System.exit(1);
        }

        // inicializa as variáveis com os parametros
        int size = Integer.parseInt(args[0]);
        int n_threads = Integer.parseInt(args[1]);

        // cria e inicializa o vetor
        V arr = new V(size);

        //arr.print();

        // reserva espaço para um vetor de threads
        Thread[] threads = new Thread[n_threads];

        // cria as threads
        for (int i = 0; i < n_threads; i++){
            threads[i] = new T(i, n_threads, arr);
        }

        // inicia as threads
        for (int i=0; i<n_threads; i++) {
            threads[i].start();
        }
        // espera pelo termino de todas as threads
        for (int i=0; i<threads.length; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return; }
        }

        // faz a soma bruta
        for (int i = 0; i < size; i++){
            sum += arr.getArray(i);
        }

        // checa se o resultado da soma esta correto
        if(arr.getSum() != sum){
            System.out.println("ERRO: Resultados não iguais.");
            System.exit(0);
        }
        System.out.println("Resultado correto.");

    }
}




