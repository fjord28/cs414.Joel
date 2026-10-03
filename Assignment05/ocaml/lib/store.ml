module StringMap = Map.Make (String)

type store = string StringMap.t

let empty = StringMap.empty

let set key value store =
  StringMap.add key value store

let delete key store =
  StringMap.remove key store

let get key store =
  StringMap.find_opt key store

let list store =
  StringMap.bindings store

let save filename store =
  let channel = open_out filename in
  Fun.protect
    (fun () ->
      StringMap.iter
        (fun key value ->
          output_string channel key;
          output_char channel '\t';
          output_string channel value;
          output_char channel '\n')
        store)
    ~finally:(fun () -> close_out_noerr channel)

let load filename =
  let channel = open_in filename in

  let rec read_lines current =
    match input_line channel with
    | line ->
        (match String.index_opt line '\t' with
        | None -> read_lines current
        | Some index ->
            let key = String.sub line 0 index in
            let value =
              String.sub line (index + 1) (String.length line - index - 1)
            in
            read_lines (set key value current))
    | exception End_of_file ->
        close_in channel;
        current
    | exception exn ->
        close_in_noerr channel;
        raise exn
  in

  try read_lines empty with
  | exn ->
      close_in_noerr channel;
      raise exn

let transaction operation store =
  match operation store with
  | Ok new_store -> new_store
  | Error _ -> store
