local money_display{
	Properties = {
		hello = {world}
		ParentEntity = {default = EntityId()}
	}
}

function money_display:OnActivate()
     -- Activation Code
      self:Properties.ParentEntity:IsValid() then
      	self.entityBusHandler
end

function money_display:OnDeactivate()
     -- Deactivation Code
end

return money_display