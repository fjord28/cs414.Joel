type 'a zipper = {
  left : 'a list;
  focus : 'a option;
  right : 'a list;
}

let create = { left = []; focus = None; right = [] }

let empty z =
  z.focus = None

let move_left z =
  match z.left, z.focus with
  | x :: xs, Some f ->
      { left = xs; focus = Some x; right = f :: z.right }
  | _ -> z

let move_right z =
  match z.right, z.focus with
  | x :: xs, Some f ->
      { left = f :: z.left; focus = Some x; right = xs }
  | _ -> z

let push_front x z =
  match z.focus with
  | None -> { left = []; focus = Some x; right = [] }
  | Some f ->
      { z with left = z.left @ [x] }

let push_back x z =
  match z.focus with
  | None -> { left = []; focus = Some x; right = [] }
  | Some _ ->
      { z with right = z.right @ [x] }

let front z =
  match List.rev z.left, z.focus with
  | x :: _, _ -> Some x
  | [], f -> f

let back z =
  match List.rev z.right, z.focus with
  | x :: _, _ -> Some x
  | [], f -> f

let focus z = z.focus

let () =
  let z = create in
  let z = push_back 10 z in
  let z = push_back 20 z in
  let z = push_front 5 z in

  let print_item = function
    | Some x -> print_int x; print_newline ()
    | None -> print_endline "Empty"
  in

  print_endline "Front:";
  print_item (front z);

  print_endline "Back:";
  print_item (back z);

  print_endline "Focus:";
  print_item (focus z);

  let z = move_right z in
  print_endline "After moving right:";
  print_item (focus z);

  let z = move_left z in
  print_endline "After moving left:";
  print_item (focus z);

  print_endline "Is empty:";
  print_endline (string_of_bool (empty z))

