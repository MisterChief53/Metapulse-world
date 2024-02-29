local start_menu_script = {
	Properties = {
		ParentEntity = {default = EntityId()}
	}
}

function start_menu_script:OnActivate()
	if(self.Properties.ParentEntitiy:IsValid()) then
		self.entityBusHandler = EntityBus.Connect(self, self.Properties.ParentEntity)
	end
end

function start_menu_script:OnDeactivate()

end

return start_menu_script