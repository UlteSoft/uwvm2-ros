-- Validate within Xmake's full rule sandbox, before loading build actions.
-- The declarative project scope intentionally has no error/raise/os.argv API.
rule("uwvm_ros_product")
    on_load(function(target)
        local product = import("utility.ros_product", {anonymous = true})
        product.uwvm_ros_product_check_current()
    end)
rule_end()
