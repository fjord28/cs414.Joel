open Store

let trim = String.trim

let split_command line =
  match String.split_on_char ' ' (trim line) with
  | [] -> ("", [])
  | command :: rest ->
      let rest = List.filter (fun part -> part <> "") rest in
      (String.uppercase_ascii command, rest)

let value_after_key line =
  let parts = String.split_on_char ' ' (trim line) in
  match List.filter (fun part -> part <> "") parts with
  | _command :: key :: rest ->
      Some (key, String.concat " " rest)
  | _ ->
      None

let print_help () =
  print_endline "Commands:";
  print_endline "  SET key value";
  print_endline "  GET key";
  print_endline "  DELETE key";
  print_endline "  LIST";
  print_endline "  SAVE filename";
  print_endline "  LOAD filename";
  print_endline "  QUIT"

let rec loop store =
  print_string "> ";
  flush stdout;

  match read_line () with
  | exception End_of_file ->
      ()
  | line ->
      let command, args = split_command line in

      match command with
      | "" ->
          loop store
      | "SET" ->
          (match value_after_key line with
          | Some (key, value) when value <> "" ->
              loop (set key value store)
          | _ ->
              print_endline "Usage: SET key value";
              loop store)
      | "GET" ->
          (match args with
          | [key] ->
              (match get key store with
              | Some value -> print_endline value
              | None -> print_endline "Key not found");
              loop store
          | _ ->
              print_endline "Usage: GET key";
              loop store)
      | "DELETE" ->
          (match args with
          | [key] ->
              if Option.is_none (get key store) then
                print_endline "Key not found";
              loop (delete key store)
          | _ ->
              print_endline "Usage: DELETE key";
              loop store)
      | "LIST" ->
          List.iter
            (fun (key, value) ->
              Printf.printf "%s = %s\n" key value)
            (list store);
          loop store
      | "SAVE" ->
          (match args with
          | [filename] ->
              (try
                 save filename store;
                 print_endline "Saved."
               with
              | Sys_error message ->
                  Printf.printf "Could not save file: %s\n" message);
              loop store
          | _ ->
              print_endline "Usage: SAVE filename";
              loop store)
      | "LOAD" ->
          (match args with
          | [filename] ->
              (try
                 let new_store = load filename in
                 print_endline "Loaded.";
                 loop new_store
               with
              | Sys_error message ->
                  Printf.printf "Could not load file: %s\n" message;
                  loop store);
          | _ ->
              print_endline "Usage: LOAD filename";
              loop store)
      | "HELP" ->
          print_help ();
          loop store
      | "QUIT" ->
          ()
      | _ ->
          print_endline "Unknown command. Type HELP for commands.";
          loop store

let () =
  print_endline "Transactional Key-Value Store";
  print_endline "Type HELP for commands.";
  loop empty
