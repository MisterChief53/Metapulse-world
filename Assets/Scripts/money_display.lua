local money_display = {
	Properties = {
		ParentEntity = {default = EntityId()}
	}
}

function money_display:OnActivate()
	if self.Properties.ParentEntity:IsValid() then
		self.entityBusHandler = EntityBus.Connect(self, self.Properties.ParentEntity)
		local interface = AuthClientInterfaceClient()
		local token = tostring(interface:RequestToken())
		UiTextBus.Event.SetText(self.Properties.ParentEntity, token)
	end
end

function money_display:OnDeactivate()
	
end

return money_display