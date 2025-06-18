/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 11 */
/* Codigo: Exemplo de uso de futures */
/* -------------------------------------------------------------------*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;


//classe runnable
class MyCallable implements Callable<Integer> {
  //construtor
  MyCallable() {
  }
 
  //método para execução
  public Integer call() throws Exception {
    Integer s = 0;
    for (Integer i=1; i<=100; i++) {
      s++;
    }
    return s;
  }
}


class Primo implements Callable<Integer> {
  int numero;
  //construtor
  Primo (int n) {
    this.numero = n;
  }
 
  //método para execução
  public Integer call() throws Exception {
    int n = this.numero;
    int i;
      if(n<=1) return 0;
      if(n==2) return 1;
      if(n%2==0) return 0;
      for(i=3; i< Math.sqrt(n)+1; i+=2) {
      if(n%i==0) return 0;
      }
      return 1;
  }
}

//classe do método main
public class FutureHello  {
  private static final int N = 10000;
  private static final int NTHREADS = 10;

  public static void main(String[] args) {
    //cria um pool de threads (NTHREADS)
    ExecutorService executor = Executors.newFixedThreadPool(NTHREADS);
    //cria uma lista para armazenar referencias de chamadas assincronas
    List<Future<Integer>> list = new ArrayList<Future<Integer>>();

    for (int i = 1; i <= N; i++) {
      Callable<Integer> worker = new Primo(i);
      Future<Integer> submit = executor.submit(worker);
      list.add(submit);
    }

    System.out.println("Checando a primalidade de " + list.size() + " numeros");
    //pode fazer outras tarefas...

    //recupera os resultados e faz o somatório final
    Integer total = 0;
    for (Future<Integer> future : list) {
      try {
        total += future.get(); //bloqueia se a computação nao tiver terminado
      } catch (InterruptedException e) {
        e.printStackTrace();
      } catch (ExecutionException e) {
        e.printStackTrace();
      }
    }
    System.out.println("entre 1 e " + N + " (inclusos) existem " + total + " numeros primos!!");
    executor.shutdown();
  }
}
