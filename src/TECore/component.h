#if !defined(COMPONENT_H)
/* ========================================================================
$File: $
$Date: $
$Revision: $
$Creator:  $
$Notice: $
======================================================================== */

#define COMPONENT_H

#include "TECore\math.h"

namespace TE
{
	namespace Core
	{
		class actor;
	}
}

namespace TE
{
	namespace Core
	{
		class component {
		public:
			virtual ~component() {};
			virtual void SetupAttachement(component* Parent) = 0;
			virtual void SetupAttachement(actor* Owner) = 0;
			virtual void Tick(float DeltaTime) = 0;
			virtual void Render(Math::mat4f Projection, Math::mat4f View) = 0;
			virtual actor* GetOwner() const = 0;
			virtual void SetChild(component* Child) = 0;
		};


		template < class derived >
		class component_CRTP : public component
		{
		public:

			component_CRTP() : _Owner(nullptr),
				_Parent(nullptr),
				_Child(nullptr)
			{}

			virtual void Tick(float DeltaTime) override
			{ 
				auto Derived = static_cast< derived* >(this);
				Derived->Tick(DeltaTime);
			}

			virtual void Render(Math::mat4f Projection, Math::mat4f View) override
			{
				auto Derived = static_cast< derived* >(this);
				Derived->Render(Projection, View);

				if (_Child)
					_Child->Render(Projection, View);
			}

			void SetupAttachement(component* Parent) override;
			void SetupAttachement(actor* Owner) override;
			actor* GetOwner() const;
			derived* AsChildType() const
			{
				return (static_cast<derived*>(this));
			}

			void SetChild(component* Child)
			{
				_Child = Child;
			}

		protected:
			actor* _Owner;
			component* _Parent;
			component* _Child;
		};

		template <class derived>
		void component_CRTP<derived>::SetupAttachement(component* Parent)
		{
			_Parent = Parent;
			_Parent->SetChild(this);
			_Owner = Parent->GetOwner();
		}

		template <class derived>
		void component_CRTP<derived>::SetupAttachement(actor* Owner)
		{
			_Owner = Owner;
		}

		template <typename derived>
		actor* component_CRTP<derived>::GetOwner() const
		{
			return (_Owner);
		}

		class default_component : public component_CRTP < default_component >
		{
		public:
			void Render(Math::mat4f Projection, Math::mat4f View) 
			{
				if (_Child)
					_Child->Render(Projection, View);
			};
		};
	}
}
#endif