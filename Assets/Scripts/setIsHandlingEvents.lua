local setIsHandlingEvents = {
	Properties = {
		ContainerElement = {default = EntityId()},
	},
}

function setIsHandlingEvents:OnActivate()
	UiInteractableBus.Event.SetIsHandlingEvents(self.Properties.ContainerElement, true)
end

function setIsHandlingEvents:OnDeactivate()
	
end


