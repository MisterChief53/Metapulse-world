local start_menu_script = {
	Properties = {
		ParentEntity = {default = EntityId()}
	}
}

function start_menu_script:OnActivate()
	local entityContext = GetEntityContext()
	local parent = entityContext:FindEntityById(EntityId())
	
	if parent ~= nil then
		local parentCommponent = parent:GetComponentOfType("UiTextComponent")
		if parentComponent  ~= nil then
			parentComponent:setText(tostring(AuthClientInterfaceClient:RequestToken()))
		end
	end
end

function start_menu_script:OnDeactivate()

end

return start_menu_script