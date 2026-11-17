(* Contract: f must not return or retain the channel for later use. *)
let with_input_file path f =
  let ch = open_in path in
  Fun.protect
    ~finally:(fun () ->
      close_in_noerr ch;
      prerr_endline ("[OCaml] released " ^ path))
    (fun () ->
      prerr_endline ("[OCaml] acquired " ^ path);
      f ch)

let contains line needle =
  let n = String.length line and m = String.length needle in
  let rec search i =
    if i > n - m then false
    else if String.sub line i m = needle then true
    else search (i + 1)
  in
  search 0

let count_matches_ocaml path needle ?(fail_after_first_line = false) () =
  if String.length needle = 0 then invalid_arg "empty search string";
  with_input_file path (fun ch ->
    let rec loop count =
      match input_line ch with
      | line ->
          let count' = if contains line needle then count + 1 else count in
          if fail_after_first_line then failwith "simulated failure";
          loop count'
      | exception End_of_file -> count
    in
    loop 0)

let with_two_input_files path1 path2 f =
  with_input_file path1 (fun ch1 ->
    with_input_file path2 (fun ch2 -> f ch1 ch2))

let usage () =
  prerr_endline "Usage: main.exe PATH NEEDLE [--fail] | --two PATH1 PATH2";
  exit 2

let () =
  try
    match Array.to_list Sys.argv with
    | [_; "--two"; path1; path2] ->
        with_two_input_files path1 path2 (fun _ _ ->
          print_endline "Both files opened")
    | [_; path; needle] ->
        let count = count_matches_ocaml path needle () in
        Printf.printf "Matches: %d\n%!" count
    | [_; path; needle; "--fail"] ->
        let count = count_matches_ocaml path needle ~fail_after_first_line:true () in
        Printf.printf "Matches: %d\n%!" count
    | _ -> usage ()
  with
  | exn ->
      prerr_endline ("Error: " ^ Printexc.to_string exn);
      exit 1
