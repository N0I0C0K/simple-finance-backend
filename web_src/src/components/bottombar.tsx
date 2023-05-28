import * as React from 'react'
import Box from '@mui/joy/Box'
import { nanoid } from 'nanoid'
import ListItemDecorator from '@mui/joy/ListItemDecorator'
import Tabs from '@mui/joy/Tabs'
import TabList from '@mui/joy/TabList'
import Tab, { tabClasses } from '@mui/joy/Tab'
import HomeOutlined from '@mui/icons-material/HomeOutlined'
import FavoriteBorder from '@mui/icons-material/FavoriteBorder'
import Search from '@mui/icons-material/Search'
import HistoryIcon from '@mui/icons-material/History'
import Person from '@mui/icons-material/Person'
import { ColorPaletteProp } from '@mui/joy'
import { useNavigate, useLocation } from 'react-router-dom'
const tabs: {
  id?: string
  icon: React.ReactNode
  label: string
  color: ColorPaletteProp
  link: string
}[] = [
  {
    icon: <HomeOutlined />,
    label: 'Home',
    color: 'primary',
    link: '/home',
  },
  {
    icon: <HistoryIcon />,
    label: 'Record',
    color: 'info',
    link: '/record',
  },
  {
    icon: <Search />,
    label: 'Settings',
    color: 'danger',
    link: '/settings',
  },
  {
    icon: <Person />,
    label: 'Profile',
    color: 'success',
    link: '/profile',
  },
]

export const BottomBar: React.FC = () => {
  const [index, setIndex] = React.useState(0)
  const goto = useNavigate()
  const path = useLocation()
  const colors = ['primary', 'info', 'danger', 'success'] as const
  React.useEffect(() => {
    const index = tabs.findIndex((tab) => tab.link === path.pathname)
    setIndex(index === -1 ? 0 : index)
  }, [path.pathname])

  return (
    <Tabs
      size='lg'
      aria-label='Bottom Navigation'
      value={index}
      onChange={(event, value) => {
        setIndex(value as number)
        goto(tabs[value as number].link)
      }}
      sx={(theme) => ({
        width: '100%',
        borderRadius: 'xl',
        maxWidth: 400,

        boxShadow: 'xl',
        '--Tabs-gap': '8px',
        '--joy-shadowChannel': theme.vars.palette[colors[index]].darkChannel,
        [`& .${tabClasses.root}`]: {
          boxShadow: 'none',
          borderRadius: 'lg',
          whiteSpace: 'nowrap',
          transition: '0.3s',
          fontWeight: 'lg',
          flex: 1,
          [`&:not(.${tabClasses.selected}):not(:hover)`]: {
            opacity: 0.72,
          },
        },
      })}
    >
      <TabList variant='plain' sx={{ '--ListItemDecorator-size': '28px' }}>
        {tabs.map((tab, i) => (
          <Tab
            key={tab.id || (tab.id = nanoid())}
            orientation='vertical'
            {...(index === i && { variant: 'soft', color: tab.color })}
          >
            <ListItemDecorator>{tab.icon}</ListItemDecorator>
            {tab.label}
          </Tab>
        ))}
      </TabList>
    </Tabs>
  )
}
