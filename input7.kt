fun factorial(n: Int): Int {
   if (n == 1){
      return 1
   } else {
      return factorial(n-1)*n
   }
}

fun main() {
   var x: Int

   x = 5;
   println(factorial(x))
}
