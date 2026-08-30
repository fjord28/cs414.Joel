type 'a rose = Node of 'a * 'a rose list

let rec size tree =
  let rec size_children children =
    match children with
    | [] -> 0
    | head :: tail ->
        size head + size_children tail
  in
  match tree with
  | Node (_, children) ->
      1 + size_children children

let rec map f tree =
  match tree with
  | Node (value, children) ->
      Node (f value, List.map (map f) children)

let rec fold f tree =
  match tree with
  | Node (value, children) ->
      f value (List.map (fold f) children)