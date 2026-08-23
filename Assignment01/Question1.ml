type nat =
  | Z
  | S of nat

let rec to_int : nat -> int = function
  | Z -> 0
  | S n -> 1 + to_int n

let rec add x y =
  match x with
  | Z -> y
  | S x_prev -> S (add x_prev y)

let rec multiply x y =
  match x with
  | Z -> Z
  | S x_prev -> add y (multiply x_prev y)

let rec sub x y =
  match x, y with
  | x, Z -> x
  | Z, _ -> Z
  | S x_prev, S y_prev -> sub x_prev y_prev

let rec less_equal x y =
  match x, y with
  | Z, _ -> true
  | _, Z -> false
  | S x_prev, S y_prev -> less_equal x_prev y_prev

let rec divide x y =
  match x, y with
  | _, Z -> failwith "Cannot divide by zero"
  | Z, _ -> (Z, Z)
  | _, _ ->
      if less_equal y x then
        let (q, r) = divide (sub x y) y in
        (S q, r)
      else
        (Z, x)