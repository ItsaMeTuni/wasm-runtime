(module
    (export "main" (func $main))
    (func $main (result i32)
        i32.const 1
        i32.const 2
        call $add)
    (func $add (param $lhs i32) (param $rhs i32) (result i32)
        local.get $lhs
        local.get $rhs
        i32.add))

