local inputScript = {
	Properties = {
		ParentEntity = {default = EntityId()}
	}
}

function inputScript:OnActivate()
	self.textInputHandler = UiTextInputNotificationBus.Connect(self, self.entityId)
end

function inputScript:OnDeactivate()
	self.textInputHandler:Disconnect()
end

return inputScript 