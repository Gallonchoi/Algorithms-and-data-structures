object Qsort {
  def sort[T <% Ordered[T]](list: List[T]): List[T] =
    list match {
      case Nil => Nil
      case k :: ks =>
        val (before, after) = ks partition(_ < k)
        sort(before) ++ (k :: sort(after))
    }

  def main(args: Array[String]) {
    val list = List(9, 8, 7, 6, 5, 4, 3, 2, 1)
    println("Before sort: " + list)
    println("After sort: " + sort(list))
  }
}
