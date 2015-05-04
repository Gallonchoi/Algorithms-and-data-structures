object HeapSort {
  def heapSort(list: Array[Int]): Array[Int] = {
    for(i <- list.length - 1 until 0 by -1) {
      swap(list, 0, i)
      heapify(list, 0, i)
    }
    list
  }

  @scala.annotation.tailrec
  private def heapify(list: Array[Int], idx: Int, max: Int) {
    (left(idx), right(idx))  match {
      case (l, r) if r < max && list(r) > list(l) && list(r) > list(idx) => {
        swap(list, idx, r)
        heapify(list, r, max)
      }
      case (l, r) if l < max && list(l) > list(idx) => {
        swap(list, idx, l)
        heapify(list, l, max)
      }
      case _ => {}
    }
  }

  private def parent(idx: Int): Int = idx / 2
  private def left(idx: Int): Int = 2 * idx + 1
  private def right(idx: Int): Int = 2 * idx + 2

  private def swap(s: Array[Int], i: Int, j: Int) {
    val v = s(i);
    s(i) = s(j);
    s(j) = v
  }

  def main(args: Array[String]) {
    val list = Array(9, 8, 7, 6, 5, 4, 3, 2, 1)
    println(list mkString(", "))
    val t = heapSort(list)
    println(t mkString(", "))
  }
}
