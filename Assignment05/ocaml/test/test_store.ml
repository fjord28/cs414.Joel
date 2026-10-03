open Store

let () =
  let store =
    empty
    |> set "x" "10"
    |> set "y" "20"
  in

  assert (get "x" store = Some "10");
  assert (get "missing" store = None);

  let store = delete "y" store in
  assert (get "y" store = None);

  let original =
    empty
    |> set "x" "10"
  in

  let aborted =
    transaction
      (fun current ->
        let modified =
          current
          |> set "x" "20"
          |> set "y" "30"
        in
        Error modified)
      original
  in

  assert (get "x" aborted = Some "10");
  assert (get "y" aborted = None);

  let committed =
    transaction
      (fun current ->
        let modified =
          current
          |> set "x" "20"
          |> set "y" "30"
        in
        Ok modified)
      original
  in

  assert (get "x" committed = Some "20");
  assert (get "y" committed = Some "30");

  let filename =
    Filename.concat (Filename.get_temp_dir_name ()) "kv_store_test.txt"
  in

  save filename committed;

  let loaded = load filename in
  assert (get "x" loaded = Some "20");
  assert (get "y" loaded = Some "30");

  Sys.remove filename
